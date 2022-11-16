static void read_extents(FILE *f, /**/ float *x, float *y, float *z) {
    int r;
    char s[2048];
    UC(efgets(s, sizeof(s), f));
    r = sscanf(s, "%f %f %f", x, y, z);
    if (r != 3)
        ERR("expect three floats, got '%s'", s);
}

static void read_grid_size(FILE *f, /**/ int *x, int *y, int *z) {
    int r;
    char s[2048];
    UC(efgets(s, sizeof(s), f));
    r = sscanf(s, "%d %d %d", x, y, z);
    if (r != 3)
        ERR("expect three integers, got '%s'", s);
}

void field_ini(const char *path, /**/ Field **pq) {
    enum {X, Y, Z};
    Field *q;
    int n, *N;
    float *ext, *D;
    FILE *f;

    EMALLOC(1, &q);
    N = q->N; ext = q->ext;
    msg_print("reading '%s'", path);
    UC(efopen(path, "r", /**/ &f));

    read_extents  (f, /**/ &ext[X], &ext[Y], &ext[Z]);
    read_grid_size(f, /**/ &N[X],     &N[Y],   &N[Z]);

    n = N[X]*N[Y]*N[Z];
    EMALLOC(n, &q->D);
    D = q->D;
    UC(efread(D, sizeof(D[0]), n, f));
    msg_print("size:   %d %d %d", N[X], N[Y], N[Z]);
    msg_print("extend: %g %g %g", ext[X], ext[Y], ext[Z]);
    msg_print("value: D[0,n-1] = %g %g", D[0], D[n-1]);

    *pq = q;
}
