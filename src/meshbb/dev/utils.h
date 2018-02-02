namespace dev {
typedef double real;
typedef double3 real3;
enum {X, Y, Z};

static __device__ bool valid(real dt0, real t) {return (t >= 0 && t <= dt0);}

// TODO belongs to scheme/ ?
// BB assumes r0 + v0 dt = r1 for now
#ifdef FORWARD_EULER
__device__ void rvprev(real dt0, const real3_t *r1, const real3_t *v1, const float *f0, /**/ real3_t *r0, real3_t *v0) {
    enum {X, Y, Z};
    v0->x = v1->x - f0[X] * dt0;
    v0->y = v1->y - f0[Y] * dt0;
    v0->z = v1->z - f0[Z] * dt0;

    r0->x = r1->x - v0->x * dt0;
    r0->y = r1->y - v0->y * dt0;
    r0->z = r1->z - v0->z * dt0;
}
#else // velocity-verlet
__device__ void rvprev(real dt0, const real3_t *r1, const real3_t *v1, const float *, /**/ real3_t *r0, real3_t *v0) {
    r0->x = r1->x - v1->x * dt0;
    r0->y = r1->y - v1->y * dt0;
    r0->z = r1->z - v1->z * dt0;

    *v0 = *v1;
}
#endif

__device__ void fetch_triangle(int id, int nt, int nv, const int4 *tt, const Particle *i_pp,
                               /**/ rPa *A, rPa *B, rPa *C) {
    int4 t;
    int tid, mid;
    tid = id % nt;
    mid = id / nt;

    t = tt[tid];
    t.x += mid * nv;
    t.y += mid * nv;
    t.z += mid * nv;

    *A = P2rP( i_pp + t.x );
    *B = P2rP( i_pp + t.y );
    *C = P2rP( i_pp + t.z );
}

__device__ void bounce_back(real dt0, const rPa *p0, const real3_t *rw, const real3_t *vw, const real_t h, /**/ rPa *pn) {
    pn->v.x = 2 * vw->x - p0->v.x;
    pn->v.y = 2 * vw->y - p0->v.y;
    pn->v.z = 2 * vw->z - p0->v.z;

    pn->r.x = rw->x + (dt0-h) * pn->v.x;
    pn->r.y = rw->y + (dt0-h) * pn->v.y;
    pn->r.z = rw->z + (dt0-h) * pn->v.z;
}

__device__ void lin_mom_change(const real3_t v0, const real3_t v1, /**/ float dP[3]) {
    dP[X] = -(v1.x - v0.x);
    dP[Y] = -(v1.y - v0.y);
    dP[Z] = -(v1.z - v0.z);
}

__device__ void ang_mom_change(const real3_t r, const real3_t v0, const real3_t v1, /**/ float dL[3]) {
    dL[X] = -(r.y * v1.z - r.z * v1.y  -  r.y * v0.z + r.z - v0.y);
    dL[Y] = -(r.z * v1.x - r.x * v1.z  -  r.z * v0.x + r.x - v0.z);
    dL[Z] = -(r.x * v1.y - r.y * v1.x  -  r.x * v0.y + r.y - v0.x);
}

/* shift origin from 0 to R for ang momentum */
__device__ void mom_shift_ref(const real3_t r, /**/ Momentum *m) {
    m->L[X] -= r.y * m->P[Z] - r.z * m->P[Y];
    m->L[Y] -= r.z * m->P[X] - r.x * m->P[Z];
    m->L[Z] -= r.x * m->P[Y] - r.y * m->P[X];
}

static __device__ bool nz(float a) {return fabs(a) > 1e-6f;}

__device__ bool nonzero(const Momentum *m) {
    return nz(m->P[X]) && nz(m->P[Y]) && nz(m->P[Z]) &&
        nz(m->L[X]) && nz(m->L[Y]) && nz(m->L[Z]);
}

enum {XX, XY, XZ, YY, YZ, ZZ};
/* see /poc/bounce-back/trianglemom.mac */

/* inertia tensor w.r.t. com of triangle */
static __device__ void compute_I(const real3_t a, const real3_t b, const real3_t c, /**/ real_t I[6]) {

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

    I[XX] = M.y + M.z;
    I[YY] = M.x + M.z;
    I[ZZ] = M.x + M.y;

    I[XY] = - inv_24 * (d.x * a.y + d.y * b.y + d.z * c.y);
    I[XZ] = - inv_24 * (d.x * a.z + d.y * b.z + d.z * c.z);
    I[YZ] = - inv_24 * (e.x * a.z + e.y * b.z + e.z * c.z);
}

static __device__ void inverse(const real_t A[6], /**/ real_t I[6]) {

    /* minors */
    const real_t mx = A[YY] * A[ZZ] - A[YZ] * A[YZ];
    const real_t my = A[XY] * A[ZZ] - A[XZ] * A[YZ];
    const real_t mz = A[XY] * A[YZ] - A[XZ] * A[YY];

    /* inverse determinant */
    real_t idet = mx * A[XX] - my * A[XY] + mz * A[XZ];
    assert( fabs(idet) > 1e-8f );
    idet = 1.f / idet;

    I[XX] =  idet * mx;
    I[XY] = -idet * my;
    I[XZ] =  idet * mz;
    I[YY] =  idet * (A[XX] * A[ZZ] - A[XZ] * A[XZ]);
    I[YZ] =  idet * (A[XY] * A[XZ] - A[XX] * A[YZ]);
    I[ZZ] =  idet * (A[XX] * A[YY] - A[XY] * A[XY]);
}

static __device__ void v2f(real dt0, const real3_t r, const real3_t om, const real3_t v, /**/ real3_t *f) {
    const float fac = rbc_mass / dt0;
    f->x = fac * (v.x + r.y * om.z - r.z * om.y);
    f->y = fac * (v.y + r.z * om.x - r.x * om.z);
    f->z = fac * (v.z + r.x * om.y - r.y * om.x);
}

__device__ void M2f(real dt0,
                    const Momentum m, real3_t a, real3_t b, real3_t c,
                    /**/ real3_t *fa, real3_t *fb, real3_t *fc) {

    real_t I[6] = {0}, Iinv[6];
    real3_t om, v, com;

    const real_t fac = 1.f / (3.f * rbc_mass);

    compute_I(a, b, c, /**/ I);
    inverse(I, /**/ Iinv);

    /* angular velocity to be added (w.r.t. com of triangle) */
    om.x = fac * (I[XX] * m.L[X] + I[XY] * m.L[Y] + I[XZ] * m.L[Z]);
    om.y = fac * (I[XY] * m.L[X] + I[YY] * m.L[Y] + I[YZ] * m.L[Z]);
    om.z = fac * (I[XZ] * m.L[X] + I[YZ] * m.L[Y] + I[ZZ] * m.L[Z]);

    /* linear velocity to be added */
    v.x =  fac * (m.P[X]);
    v.y =  fac * (m.P[Y]);
    v.z =  fac * (m.P[Z]);

    /* referential is com of triangle, shift it */
    com.x = 0.333333f * (a.x + b.x + c.x);
    com.y = 0.333333f * (a.y + b.y + c.y);
    com.z = 0.333333f * (a.z + b.z + c.z);

    a.x -= com.x;
    a.y -= com.y;
    a.z -= com.z;

    v2f(dt0, a, om, v, /**/ fa);
    v2f(dt0, b, om, v, /**/ fb);
    v2f(dt0, c, om, v, /**/ fc);
}

} // dev
