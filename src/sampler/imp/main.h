static long get_size(const Grid *g) {
    int3 N = g->N;
    return N.x * N.y * N.z;
}

static void ini_dev_grid(int3 L, int3 M, int3 N, Grid *g) {
    long n;
    g->N = N;
    g->L = L;

    n = get_size(g);
    Dalloc(&g->pp, n);
    Dalloc(&g->ss, 6*n);
}

static void ini_hst_grid(int3 L, int3 M, int3 N, Grid *g) {
    long n;
    g->N = N;
    g->L = L;

    n = get_size(g);
    EMALLOC(n,   &g->pp);
    EMALLOC(6*n, &g->ss);
}

static void fin_dev_grid(Grid *g) {
    Dfree(g->pp);
    Dfree(g->ss);
}

static void fin_hst_grid(Grid *g) {
    EFREE(g->pp);
    EFREE(g->ss);
}

void sampler_ini(int3 L, int3 M, int3 N, Sampler **s0) {
    Sampler *s;
    EMALLOC(1, s0);
    s = *s0;
    UC(ini_dev_grid(L, M, N, &s->dev));
    UC(ini_hst_grid(L, M, N, &s->hst));
    UC(sampler_reset(s));
}

void sampler_fin(Sampler *s) {
    UC(fin_dev_grid(&s->dev));
    UC(fin_hst_grid(&s->hst));
    EFREE(s);
}

static void reset_dev_grid(Grid *g) {
    long n = get_size(g);
    DzeroA(g->pp, n);
    DzeroA(g->ss, 6*n);
}

void sampler_reset(Sampler *s) {
    s->nsteps = 0;
    UC(reset_dev_grid(&s->dev));
}

void sampler_add(const SampleData *data, Sampler *s) {
    long i, n;
    SampleDatum d;
    Grid g = s->dev;
    for (i = 0; i < data->n; ++i) {
        d = data->d[i];
        n = d.n;
        KL(sampler_dev::add, (k_cnf(n)), (d, g));
    }
    ++ s->nsteps;
}

static void avg(int nsteps, Grid *g) {
    long n = get_size(g);
    KL(sampler_dev::avg, (k_cnf(n)), (nsteps, *g));
}

static void download(const Grid *dev, Grid *hst) {
    long n = get_size(dev);
    aD2H(dev->pp, hst->pp,   n);
    aD2H(dev->ss, hst->ss, 6*n);
}

void sampler_dump(Sampler *s) {
    UC(avg(s->nsteps, &s->dev));
    UC(download(&s->dev, &s->hst));
    
}
