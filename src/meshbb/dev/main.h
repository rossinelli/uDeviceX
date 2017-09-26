namespace dev {

static __device__ int3 get_cidx(int3 L, const float r[3]) {
    enum {X, Y, Z};
    int3 c;
    c.x = floor((double) r[X] + L.x/2);
    c.y = floor((double) r[Y] + L.y/2);
    c.z = floor((double) r[Z] + L.z/2);

    c.x = min(L.x, max(0, c.x));
    c.y = min(L.y, max(0, c.y));
    c.z = min(L.z, max(0, c.z));
    return c;
}

static __device__ int min3(int a, int b, int c) {return min(a, min(b, c));}
static __device__ int max3(int a, int b, int c) {return max(a, max(b, c));}

static __device__ int3 min3(int3 a, int3 b, int3 c) {
    return make_int3(min3(a.x, b.x, c.x),
                     min3(a.y, b.y, c.y),
                     min3(a.z, b.z, c.z));
}

static __device__ int3 max3(int3 a, int3 b, int3 c) {
    return make_int3(max3(a.x, b.x, c.x),
                     max3(a.y, b.y, c.y),
                     max3(a.z, b.z, c.z));
}

static __device__ void get_cells(int3 L, const Particle *A, const Particle *B, const Particle *C,
                          /**/ int3 *lo, int3 *hi) {
    int3 ca, cb, cc;
    ca = get_cidx(L, A->r);
    cb = get_cidx(L, B->r);
    cc = get_cidx(L, C->r);

    *lo = min3(ca, cb, cc);
    *hi = max3(ca, cb, cc);
}

static __device__ void find_collisions_cell(int tid, int start, int count, const Particle *pp, const Force *ff,
                                            const Particle *A, const Particle *B, const Particle *C,
                                            /**/ int *ncol, float4 *datacol, int *idcol) {
    int i, entry;
    Particle p, p0; Force f;
    BBState state;
    float tc, u, v;
    
    for (i = start; i < start + count; ++i) {
        p = pp[i];
        f = ff[i];
        rvprev(p.r, p.v, f.f, /**/ p0.r, p0.v);

        state = intersect_triangle(A->r, B->r, C->r, A->v, B->v, C->v, &p0, /*io*/ &tc, /**/ &u, &v);

        dbg::log_states(state);
        
        if (state == BB_SUCCESS) {
            entry = atomicAdd(ncol + i, 1);
            entry += i * MAX_COL;
            datacol[entry] = make_float4(tc, u, v, 0);
            idcol[entry] = tid;
        }            
    }
}

static __device__ void revert(float h, Particle *p) {
    enum {X, Y, Z};
    p->r[X] -= p->v[X] * h;
    p->r[Y] -= p->v[Y] * h;
    p->r[Z] -= p->v[Z] * h;
}

__global__ void find_collisions(int nm, int nt, const int4 *tt, const Particle *i_pp,
                                int3 L, const int *starts, const int *counts, const Particle *pp, const Force *ff,
                                /**/ int *ncol, float4 *datacol, int *idcol) {
    
    int gid, mid, tid, ix, iy, iz, cid;
    Particle A, B, C;
    int4 tri;
    int3 str, end;
    gid = threadIdx.x + blockIdx.x * blockDim.x;

    if (gid >= nm * nt) return;

    mid = gid / nt;
    tid = gid % nt;

    tri = tt[tid];

    A = i_pp[mid * nt + tri.x];
    B = i_pp[mid * nt + tri.y];
    C = i_pp[mid * nt + tri.z];

    revert(dt, &A);
    revert(dt, &B);
    revert(dt, &C);

    get_cells(L, &A, &B, &C, /**/ &str, &end);
    
    for (iz = str.z; iz < end.z; ++iz) {
        for (iy = str.y; iy < end.y; ++iy) {
            for (ix = str.x; ix < end.x; ++ix) {
                cid = ix + L.x * (iy + L.y * iz);
                
                find_collisions_cell(tid, starts[cid], counts[cid], pp, ff, &A, &B, &C, /**/ ncol, datacol, idcol);
            }
        }
    }
}

__global__ void select_collisions(int n, /**/ int *ncol, float4 *datacol, int *idcol) {
    int i, c, j, dst, src, argmin;
    float tmin;
    float4 d;
    i = threadIdx.x + blockIdx.x * blockDim.x;

    if (i >= n) return;

    c = ncol[i];

    argmin = 0; tmin = 2*dt;
    for (j = 0; j < c; ++j) {
        src = MAX_COL * i + j;
        d = datacol[src];
        if (d.x < tmin) {
            argmin = j;
            tmin = d.x;
        }
    }

    if (c) {
        src = MAX_COL * i + argmin;
        dst = MAX_COL * i;
        idcol[dst]   = idcol[src];
        datacol[dst] = datacol[src];
        ncol[i] = 1; /* we can use find_collisions again */
    }
}

static __device__ void get_collision_point(const float4 dcol, int id, int nt, const int4 *tt, const Particle *i_pp,
                                           /**/ float rw[3], float vw[3]) {
    enum {X, Y, Z};
    int4 t;
    Particle A, B, C;
    int tid, mid;
    float h, u, v, w;
    tid = id % nt;
    mid = id / nt;

    t = tt[tid];
    A = i_pp[mid * nt + t.x];
    B = i_pp[mid * nt + t.y];
    C = i_pp[mid * nt + t.z];

    /* d.x is collision time */
    h = dt - dcol.x;
    revert(h, &A);
    revert(h, &B);
    revert(h, &C);

    u = dcol.y;
    v = dcol.z;
    w = 1.f - u - v;

    rw[X] = w * A.r[X] + u * B.r[X] + v * C.r[X];
    rw[Y] = w * A.r[Y] + u * B.r[Y] + v * C.r[Y];
    rw[Z] = w * A.r[Z] + u * B.r[Z] + v * C.r[Z];

    vw[X] = w * A.v[X] + u * B.v[X] + v * C.v[X];
    vw[Y] = w * A.v[Y] + u * B.v[Y] + v * C.v[Y];
    vw[Z] = w * A.v[Z] + u * B.v[Z] + v * C.v[Z];    
}

__global__ void perform_collisions(int n, const int *ncol, const float4 *datacol, const int *idcol,
                                   const Force *ff, int nt, const int4 *tt, const Particle *i_pp,
                                   /**/ Particle *pp, Momentum *mm) {
    int i, id, entry;
    float4 d;
    Particle p1, p0, pn;
    Force f;
    float rw[3], vw[3];
    Momentum m;
    i = threadIdx.x + blockIdx.x * blockDim.x;

    if (i >= n || ncol[i] == 0) return;

    entry = i * MAX_COL;
    id = idcol[entry];
    d  = datacol[entry];

    p1 = pp[i];
    f  = ff[i];
    
    rvprev(p1.r, p1.v, f.f, /**/ p0.r, p0.v);

    get_collision_point(d, id, nt, tt, i_pp, /**/ rw, vw);

    bounce_back(&p0, rw, vw, d.x, /**/ &pn);
    pp[i] = pn;

    /* add momentum */

    lin_mom_change(    p1.v, pn.v, /**/ m.P);
    ang_mom_change(rw, p1.v, pn.v, /**/ m.L);

    atomicAdd(mm[id].P + X, m.P[X]);
    atomicAdd(mm[id].P + Y, m.P[Y]);
    atomicAdd(mm[id].P + Z, m.P[Z]);

    atomicAdd(mm[id].L + X, m.L[X]);
    atomicAdd(mm[id].L + Y, m.L[Y]);
    atomicAdd(mm[id].L + Z, m.L[Z]);
}

} // dev
