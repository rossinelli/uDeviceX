static void mult(double A[16], const double a[4], /**/ double b[4]) {
    /* b = A x a */
    enum {X, Y, Z, W};    
    int c, i;
    i = 0;
    for (c = 0; c < 4; c++) {
        b[c]  = A[i++]*a[X];
        b[c] += A[i++]*a[Y];
        b[c] += A[i++]*a[Z];
        b[c] += A[i++]*a[W];
    }
}

static void homogenius_multi(double A[16], const float a0[3], /**/ float b0[3]) {
    /* b = A x [a, 1] */
    enum {X, Y, Z, W};
    double a[4], b[4];
    a[X] = a0[X]; a[Y] = a0[Y]; a[Z] = a0[Z]; a[W] = 1;
    mult(A, a, /**/ b);
    b0[X] = b[X]; b0[Y] = b[Y]; b0[Z] = b[Z];
}

static void gen0(double A[16], const float *r0, Particle *p) {
    enum {X, Y, Z};
    p->v[X] = p->v[Y] = p->v[Z] = 0;
    homogenius_multi(A, r0, /**/ p->r);
}

static void gen1(double A[16], int nv, const float *rr, Particle *pp) {
    int i;
    for (i = 0; i < nv; i++) gen0(A, &rr[3*i], &pp[i]);
}
void rbc_gen0(int nv, const float *rr, const Matrices *matrices, /**/ int *pn, Particle *pp) {
    int i, n, nm;
    double *A;
    n = 0;
    nm = matrices_get_n(matrices);
    for (i = 0; i < nm; i++) {
        matrices_get(matrices, i, &A);
        gen1(A, nv, rr, &pp[n]); n += nv;
    }
    *pn = n;
}

void rbc_shift(const Coords *c, int n, Particle *pp) {
}