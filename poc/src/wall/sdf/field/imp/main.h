static void dump0(const Coords *coords, const int N0[3], const float *D0, /**/ float *D1) {
    int L[3] = {xs(coords), ys(coords), zs(coords)};
    Tform *t;
    UC(tform_ini(&t));
    UC(out2sdf_ini(coords, N0, /**/ t));
    UC(sample(t, N0, D0,   L, /**/ D1));
    UC(tform_fin(t));
}

static void dump1(const Coords *coords, MPI_Comm cart, const int N[3], const float *D, /*w*/ float *W) {
    int3 L = subdomain(coords);
    const char *names[] = {"wall"};
    const float *data[] = {W};

    UC(dump0(coords, N, D, /**/ W));
    UC(grid_write(L, L, cart, DUMP_BASE "/h5/wall.h5", 1, data, names));
}

static void dump(const Coords *coords, MPI_Comm cart, const int N[], const float *D) {
    float *W;
    int ngrid = xs(coords) * ys(coords) * zs(coords);
    EMALLOC(ngrid, &W);
    UC(dump1(coords, cart, N, D, /*w*/ W));
    EFREE(W);
}

void field_size(const Field *q, /**/ int N[3]) {
    enum {X, Y, Z};
    N[X] = q->N[X]; N[Y] = q->N[Y]; N[Z] = q->N[Z];
}
void field_extend(const Field *q, /**/ float ext[3]) {
    enum {X, Y, Z};
    ext[X] = q->ext[X]; ext[Y] = q->ext[Y]; ext[Z] = q->ext[Z];
}
void field_data(const Field *q, /**/ float **pD) {
    *pD = q->D;
}
void field_scale(Field *q, float scale) {
    enum {X, Y, Z};
    float *D;
    int i, n, *N;
    N = q->N; D = q->D;
    n = N[X]*N[Y]*N[Z];
    for (i = 0; i < n; i++) D[i] *= scale;
}

void field_sample(const Field *F, const Tform *t, const int N1[3], /**/ Field **pq) {
    enum {X, Y, Z};
    int n;
    Field *q;
    EMALLOC(1, &q);
    n = N1[X]*N1[Y]*N1[Z];
    EMALLOC(n, &q->D);
    sample(t, F->N, F->D, N1, /**/ q->D);
    q->N[X] = N1[X];
    q->N[Y] = N1[Y];
    q->N[Z] = N1[Z];
    /* q->ext ?  */
    *pq = q;
}

void field_dump(const Field *q, const Coords *c, MPI_Comm cart) {
    dump(c, cart, q->N, q->D);
}
