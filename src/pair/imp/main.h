void pair_ini(PairParams **par) {
    PairParams *p;
    UC(emalloc(sizeof(PairParams), (void **) par));
    p = *par;

    p->ncolors = 0;
}

void pair_fin(PairParams *p) {
    UC(efree(p));
}

void pair_set_lj(float sigma, float eps, PairParams *p) {
    p->ljs = sigma;
    p->lje = eps;
}

static int get_npar(int ncol) { return (ncol * (ncol+1)) / 2; }

void pair_set_dpd(int ncol, const float a[], const float g[], const float s[], PairParams *p) {
    int npar = get_npar(ncol);
    size_t sz = sizeof(float) * npar;

    if (ncol <= 0)      ERR("need at least one color");
    if (ncol > MAX_COL) ERR("too many colors");
    
    p->ncolors = ncol;
    memcpy(p->a, a, sz);
    memcpy(p->g, g, sz);
    memcpy(p->s, s, sz);
}

void pair_get_view_dpd(const PairParams *p, PairDPD *v) {
    enum {PID=0};
    v->a = p->a[PID];
    v->g = p->g[PID];
    v->s = p->s[PID];
}

void pair_get_view_dpd_color(const PairParams *p, PairDPDC *v) {
    size_t sz;
    v->ncolors = p->ncolors;
    sz = p->ncolors * sizeof(float);
    memcpy(v->a, p->a, sz);
    memcpy(v->g, p->g, sz);
    memcpy(v->s, p->s, sz);
}

void pair_get_view_dpd_lj(const PairParams *p, PairDPDLJ *v) {
    enum {PID=0};
    v->a = p->a[PID];
    v->g = p->g[PID];
    v->s = p->s[PID];
    v->ljs = p->ljs;
    v->lje = p->lje;
}