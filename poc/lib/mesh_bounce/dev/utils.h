typedef double real;
typedef double3 real3;
enum {X, Y, Z};


template <typename T> _S_ T min3(T a, T b, T c) {return min(a, min(b, c));}
template <typename T> _S_ T max3(T a, T b, T c) {return max(a, max(b, c));}

template <typename T3>
_I_ T3 min3T3(T3 a, T3 b, T3 c) {
    T3 v;
    v.x = min3(a.x, b.x, c.x);
    v.y = min3(a.y, b.y, c.y);
    v.z = min3(a.z, b.z, c.z);
    return v;
}

template <typename T3>
_I_ T3 max3T3(T3 a, T3 b, T3 c) {
    T3 v;
    v.x = max3(a.x, b.x, c.x);
    v.y = max3(a.y, b.y, c.y);
    v.z = max3(a.z, b.z, c.z);
    return v;
}

_I_ int3 get_cidx(int3 L, real3_t r) {
    int3 c;
    c.x = floor((double) r.x + L.x/2);
    c.y = floor((double) r.y + L.y/2);
    c.z = floor((double) r.z + L.z/2);

    c.x = min(L.x-1, max(0, c.x));
    c.y = min(L.y-1, max(0, c.y));
    c.z = min(L.z-1, max(0, c.z));
    return c;
}


_I_ bool valid_time(real dt, real t) {return (t >= 0 && t <= dt);}

// BB assumes r0 + v0 dt = r1
_I_ void vprev(real dt, const real3_t *r1, const real3_t *r0, /**/ real3_t *v0) {
    v0->x = (r1->x - r0->x) / dt;
    v0->y = (r1->y - r0->y) / dt;
    v0->z = (r1->z - r0->z) / dt;
}

_I_ void fetch_triangle(int id, int nt, int nv, const int4 *tt, const Particle *i_pp,
                        /**/ rPa *A, rPa *B, rPa *C) {
    int4 t;
    int tid, mid;
    tid = id % nt;
    mid = id / nt;

    t = tt[tid];
    t.x += mid * nv;
    t.y += mid * nv;
    t.z += mid * nv;

    *A = fetch_Part(t.x, i_pp);
    *B = fetch_Part(t.y, i_pp);
    *C = fetch_Part(t.z, i_pp);
}

_I_ void fetch_triangle(float dt, int id, int nt, int nv, const int4 *tt, const Positioncp *i_rr,
                        /**/ rPa *A, rPa *B, rPa *C) {
    int4 t;
    int tid, mid;
    tid = id % nt;
    mid = id / nt;

    t = tt[tid];
    t.x += mid * nv;
    t.y += mid * nv;
    t.z += mid * nv;

    *A = fetch_Part(dt, t.x, i_rr);
    *B = fetch_Part(dt, t.y, i_rr);
    *C = fetch_Part(dt, t.z, i_rr);
}

_I_ void bounce_back(real dt, const rPa *p0, const real3_t *rw, const real3_t *vw, const real_t h, /**/ rPa *pn) {
    pn->v.x = 2 * vw->x - p0->v.x;
    pn->v.y = 2 * vw->y - p0->v.y;
    pn->v.z = 2 * vw->z - p0->v.z;

    pn->r.x = rw->x + (dt-h) * pn->v.x;
    pn->r.y = rw->y + (dt-h) * pn->v.y;
    pn->r.z = rw->z + (dt-h) * pn->v.z;
}

_I_ void lin_mom_change(float m, const real3_t v0, const real3_t v1, /**/ float dP[3]) {
    dP[X] = - m * (v1.x - v0.x);
    dP[Y] = - m * (v1.y - v0.y);
    dP[Z] = - m * (v1.z - v0.z);
}

_I_ void ang_mom_change(float m, real3_t rc, real3_t r0, real3_t v0, real3_t r1, real3_t v1, /**/ float dL[3]) {
    r0.x -= rc.x; r0.y -= rc.y; r0.z -= rc.z;
    r1.x -= rc.x; r1.y -= rc.y; r1.z -= rc.z;
    
    dL[X] = - m * (r1.y * v1.z - r1.z * v1.y  -  r0.y * v0.z + r0.z - v0.y);
    dL[Y] = - m * (r1.z * v1.x - r1.x * v1.z  -  r0.z * v0.x + r0.x - v0.z);
    dL[Z] = - m * (r1.x * v1.y - r1.y * v1.x  -  r0.x * v0.y + r0.y - v0.x);
}

/* shift origin from 0 to R for ang momentum */
_I_ void mom_shift_ref(const real3_t r, /**/ Momentum *m) {
    m->L[X] -= r.y * m->P[Z] - r.z * m->P[Y];
    m->L[Y] -= r.z * m->P[X] - r.x * m->P[Z];
    m->L[Z] -= r.x * m->P[Y] - r.y * m->P[X];
}

_S_ bool nz(float a) {return fabs(a) > 1e-6f;}

_I_ bool nonzero(const Momentum *m) {
    return nz(m->P[X]) && nz(m->P[Y]) && nz(m->P[Z]) &&
        nz(m->L[X]) && nz(m->L[Y]) && nz(m->L[Z]);
}

enum {XX, XY, XZ, YY, YZ, ZZ};
/* see /poc/bounce-back/trianglemom.mac */

/* inertia tensor w.r.t. com of triangle */
_S_ void compute_I(float mass, const real3_t a, const real3_t b, const real3_t c, /**/ real_t I[6]) {

    real3_t M, d, e;
    const real_t inv_12 = 1.f/12.f;
    const real_t inv_24 = 1.f/24.f;

    M.x = inv_12 * (a.x * a.x - a.x * b.x + b.x * b.x - c.x * (a.x + b.x) + c.x * c.x);
    M.y = inv_12 * (a.y * a.y - a.y * b.y + b.y * b.y - c.y * (a.y + b.y) + c.y * c.y);
    M.z = inv_12 * (a.z * a.z - a.z * b.z + b.z * b.z - c.z * (a.z + b.z) + c.z * c.z);

    d.x = 2 * a.x - b.x - c.x;
    d.y = 2 * b.x - a.x - c.x;
    d.z = 2 * c.x - a.x - b.x;

    e.x = 2 * a.y - b.y - c.y;
    e.y = 2 * b.y - a.y - c.y;
    e.z = 2 * c.y - a.y - b.y;

    I[XX] = mass * (M.y + M.z);
    I[YY] = mass * (M.x + M.z);
    I[ZZ] = mass * (M.x + M.y);

    I[XY] = - inv_24 * mass * (d.x * a.y + d.y * b.y + d.z * c.y);
    I[XZ] = - inv_24 * mass * (d.x * a.z + d.y * b.z + d.z * c.z);
    I[YZ] = - inv_24 * mass * (e.x * a.z + e.y * b.z + e.z * c.z);
}

_S_ void inverse(const real_t A[6], /**/ real_t I[6]) {

    /* minors */
    const real_t mx = A[YY] * A[ZZ] - A[YZ] * A[YZ];
    const real_t my = A[XY] * A[ZZ] - A[XZ] * A[YZ];
    const real_t mz = A[XY] * A[YZ] - A[XZ] * A[YY];

    /* inverse determinant */
    real_t idet = mx * A[XX] - my * A[XY] + mz * A[XZ];
    //assert( fabs(idet) > 1e-8f );
    idet = 1.f / idet;

    I[XX] =  idet * mx;
    I[XY] = -idet * my;
    I[XZ] =  idet * mz;
    I[YY] =  idet * (A[XX] * A[ZZ] - A[XZ] * A[XZ]);
    I[YZ] =  idet * (A[XY] * A[XZ] - A[XX] * A[YZ]);
    I[ZZ] =  idet * (A[XX] * A[YY] - A[XY] * A[XY]);
}

_S_ void rigid_motion_vel(const real3_t r, const real3_t om, const real3_t v0, /**/ real3_t *v) {
    v->x = v0.x + r.y * om.z - r.z * om.y;
    v->y = v0.y + r.z * om.x - r.x * om.z;
    v->z = v0.z + r.x * om.y - r.y * om.x;
}

_I_ void rbc_M2v(float mass, const Momentum m, real3_t a, real3_t b, real3_t c,
                 /**/ real3_t *va, real3_t *vb, real3_t *vc) {

    real_t I[6], Iinv[6], inv_mass;
    real3_t om, v, com;

    const real_t one_third = 1.f / 3.f;

    compute_I(mass, a, b, c, /**/ I);
    inverse(I, /**/ Iinv);

    /* angular velocity to be added (w.r.t. com of triangle) */
    om.x = one_third * (Iinv[XX] * m.L[X] + Iinv[XY] * m.L[Y] + Iinv[XZ] * m.L[Z]);
    om.y = one_third * (Iinv[XY] * m.L[X] + Iinv[YY] * m.L[Y] + Iinv[YZ] * m.L[Z]);
    om.z = one_third * (Iinv[XZ] * m.L[X] + Iinv[YZ] * m.L[Y] + Iinv[ZZ] * m.L[Z]);

    /* linear velocity to be added */
    inv_mass = 1.0 / mass;
    v.x =  one_third * m.P[X] * inv_mass;
    v.y =  one_third * m.P[Y] * inv_mass;
    v.z =  one_third * m.P[Z] * inv_mass;

    /* referential is com of triangle, shift it */
    com.x = one_third * (a.x + b.x + c.x);
    com.y = one_third * (a.y + b.y + c.y);
    com.z = one_third * (a.z + b.z + c.z);

    a.x -= com.x; b.x -= com.x; c.x -= com.x;
    a.y -= com.y; b.y -= com.y; c.y -= com.y;
    a.z -= com.z; b.z -= com.z; c.z -= com.z;

    rigid_motion_vel(a, om, v, va);
    rigid_motion_vel(b, om, v, vb);
    rigid_motion_vel(c, om, v, vc);
}
