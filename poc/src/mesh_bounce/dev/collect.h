_S_ void rig_transform_mom(real3_t dr, Momentum *m) {
    mom_shift_ref(dr, /**/ m);

    m->L[X] += dr.y * m->P[Z] - dr.z * m->P[Y];
    m->L[Y] += dr.z * m->P[X] - dr.x * m->P[Z];
    m->L[Z] += dr.x * m->P[Y] - dr.y * m->P[X];    
}

_S_ void rig_add_lin_mom(float mass, const float P[3], float *v) {
    enum {X, Y, Z};
    float fac = 1.0 / mass;
    atomicAdd(&v[X], fac * P[X]);
    atomicAdd(&v[Y], fac * P[Y]);
    atomicAdd(&v[Z], fac * P[Z]);
}

_S_ void rig_add_ang_mom(const float Iinv[], const float L[3], float *om) {
    enum {X, Y, Z, D};
    enum {XX, XY, XZ, YY, YZ, ZZ};
    enum {YX = XY, ZX = XZ, ZY = YZ};
    float dom[D] = {
        Iinv[XX] * L[X] + Iinv[XY] * L[Y] + Iinv[XZ] * L[Z],
        Iinv[YX] * L[X] + Iinv[YY] * L[Y] + Iinv[YZ] * L[Z],
        Iinv[ZX] * L[X] + Iinv[ZY] * L[Y] + Iinv[ZZ] * L[Z]
    };
    
    atomicAdd(&om[X], dom[X]);
    atomicAdd(&om[Y], dom[Y]);
    atomicAdd(&om[Z], dom[Z]);
}

/* assume very small portion of non zero momentum changes */
__global__ void collect_rig_mom(int ns, int nt, int nv, const int4 *tt, const Particle *pp, const Momentum *mm, /**/ Rigid *ss) {
    int i, sid;
    Rigid *s;
    i = threadIdx.x + blockDim.x * blockIdx.x;

    sid = i / nt;

    if (sid >= ns) return;

    Momentum m = mm[i];
    s = &ss[sid];

    if (nonzero(&m)) {
        rPa A, B, C;
        real3_t dr;
        
        fetch_triangle(i, nt, nv, tt, pp, /**/ &A, &B, &C);

        dr.x = s->com[X];
        dr.y = s->com[Y];
        dr.z = s->com[Z];
        
        dr.x -= 0.333333 * (A.r.x + B.r.x + C.r.x);
        dr.y -= 0.333333 * (A.r.y + B.r.y + C.r.y);
        dr.z -= 0.333333 * (A.r.z + B.r.z + C.r.z);

        rig_transform_mom(dr, &m);        

        rig_add_lin_mom(s->mass, m.P, s->v);
        rig_add_ang_mom(s->Iinv, m.L, s->om);
    }
}

_S_ void addVel(const real3_t v, int i, Particle *pp) {
    enum {X, Y, Z};
    atomicAdd(pp[i].v + X, v.x);
    atomicAdd(pp[i].v + Y, v.y);
    atomicAdd(pp[i].v + Z, v.z);
}

__global__ void collect_rbc_mom(float mass, int nc, int nt, int nv, const int4 *tt, const Momentum *mm, /**/ Particle *pp) {
    int i, cid, tid;
    int4 t;
    i = threadIdx.x + blockDim.x * blockIdx.x;

    tid = i % nt;
    cid = i / nt;

    if (cid >= nc) return;

    Momentum m = mm[i];

    if (nonzero(&m)) {
        rPa A, B, C;
        real3_t va, vb, vc;

        t = tt[tid];
        t.x += cid * nv;
        t.y += cid * nv;
        t.z += cid * nv;
        
        A = fetch_Part(t.x, pp);
        B = fetch_Part(t.y, pp);
        C = fetch_Part(t.z, pp);

        rbc_M2v(mass, m, A.r, B.r, C.r, /**/ &va, &vb, &vc);

        addVel(va, t.x, /**/ pp);
        addVel(vb, t.y, /**/ pp);
        addVel(vc, t.z, /**/ pp);
    }
}
