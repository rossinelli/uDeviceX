static void assert_np(int n, int m) {
    if (n > m) ERR("too many particles: n = %d < m = %d", n, m);
}

static int gen0(int3 L, int numdensity, int maxp, Particle *pp) { /* generate particle positions and velocities */
    enum {X, Y, Z};
    UC(assert_np(L.x * L.y * L.z * numdensity, maxp));
    os_srand(123456);
    int iz, iy, ix, l, nd = numdensity;
    int n = 0; /* particle index */
    float x, y, z, dr = 0.99;
    for (iz = 0; iz < L.z; iz++)
    for (iy = 0; iy < L.y; iy++)
    for (ix = 0; ix < L.x; ix++) {
        /* edge of a cell */
        int xlo = -0.5*L.x + ix, ylo = -0.5*L.y + iy, zlo = -0.5*L.z + iz;
        for (l = 0; l < nd; l++) {
            Particle p;
            x = xlo + dr * os_drand(), y = ylo + dr * os_drand(), z = zlo + dr * os_drand();
            p.r[X] = x; p.r[Y] = y; p.r[Z] = z;
            p.v[X] = 0; p.v[Y] = 0; p.v[Z] = 0;
            pp[n++] = p;
        }
    }
    msg_print("ic::gen: %d solvent particles", n);
    return n;
}

static int genColor(int3 L, int numdensity, int maxp, const Coords *coords, const GenColor *gc, /*o*/ Particle *pp, int *color, /*w*/ Particle *pp_hst, int *color_hst) {
    int n = gen0(L, numdensity, maxp, pp_hst);
    inter_color_apply_hst(coords, gc, n, pp_hst, /**/ color_hst);
    cH2D(color, color_hst, n);
    cH2D(   pp,    pp_hst, n);
    return n;
}

static int genGrey(int3 L, int numdensity, int maxp, /*o*/ Particle *dev, /*w*/ Particle *hst) {
    int n = gen0(L, numdensity, maxp, hst);
    cH2D(dev, hst, n);
    return n;
}

void flu_gen_quants(const Coords *coords, int numdensity, const GenColor *gc, FluQuants *q) {
    int3 L = subdomain(coords);
    if (q->colors)
        q->n = genColor(L, numdensity, q->maxp, coords, gc, q->pp, q->cc, /*w*/ q->pp_hst, q->cc_hst);
    else
        q->n = genGrey(L, numdensity, q->maxp, q->pp, /*w*/ q->pp_hst);
}

static void ii_gen0(MPI_Comm comm, const long n, int *ii) {
    long i0 = 0;
    MC(m::Exscan(&n, &i0, 1, MPI_LONG, MPI_SUM, comm));
    for (long i = 0; i < n; ++i) ii[i] = i + i0;
}

static void ii_gen(MPI_Comm comm, const int n, int *ii_dev, int *ii_hst) {
    ii_gen0(comm, n, ii_hst);
    cH2D(ii_dev, ii_hst, n);
}

void flu_gen_ids(MPI_Comm comm, const int n, FluQuants *q) {
    ii_gen(comm, n, q->ii, q->ii_hst);
}
