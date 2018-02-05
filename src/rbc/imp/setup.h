static void setup_edg(Adj *adj, /**/ float *a_dev, float *A_dev, float *totArea) {
    const char *path = "rbc.stress.free";
    int n;
    const float *rr;
    float *a_hst, *A_hst;
    OffRead *cell;
    RbcShape *shape;
    UC(off_read(path, &cell));
    rr = off_get_vert(cell);
    UC(rbc_shape_ini(adj, rr, /**/ &shape));
    n = adj_get_max(adj);

    rbc_shape_edg(shape, &a_hst);
    rbc_shape_area(shape, &A_hst);
    rbc_shape_total_area(shape, totArea);

    cH2D(a_dev, a_hst, n);
    cH2D(A_dev, A_hst, n);

    UC(rbc_shape_fin(shape));
    UC(off_fin(cell));
}

static void setup_anti(Adj *adj, /**/ int *dev) {
    int n;
    int *hst;
    n = adj_get_max(adj);
    EMALLOC(n, &hst);
    adj_get_anti(adj, /**/ hst);
    cH2D(dev, hst, n);
    EFREE(hst);
}

static void setup0(Adj *adj, /**/ int *anti, float *a, float *A, float *totArea) {
    if (RBC_STRESS_FREE) UC(setup_edg(adj, /**/ a, A, totArea));
    if (RBC_RND)         UC(setup_anti(adj, /**/ anti));
}

static void setup(int md, int nt, int nv, const int4 *tt, /**/ RbcQuants *q) {
    Adj *adj;
    UC(adj_ini(md, nt, nv, tt, /**/ &adj));
    UC(adj_view_ini(adj, /**/ &q->adj_v));
    UC(setup0(adj, /**/ q->shape.anti, q->shape.a, q->shape.A, &q->shape.totArea));
    UC(adj_fin(adj));
}
