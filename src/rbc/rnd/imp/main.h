static void ini0(RbcRnd *d, int n, long seed) {
    Dalloc(&d->r, n);
    CU(rnd_api::CreateGenerator(&d->g));
    seed = decode_seed(seed);
    CU(rnd_api::SetPseudoRandomGeneratorSeed(d->g,  seed));
    d->max = n;
}
void rbc_rnd_ini(RbcRnd **pd, int n, long seed) {
    RbcRnd* d;
    UC(emalloc(sizeof(RbcRnd), (void**) &d));
    ini0(d, n, seed);
    *pd = d;
}

static void fin0(RbcRnd *d) {
    Dfree(d->r);
    CU(rnd_api::DestroyGenerator(d->g));
}
void rbc_rnd_fin(RbcRnd *d) {
    fin0(d);
    free(d);
}

static void assert_n(int n, int max, const char *s) {
    if (n > max)
        ERR("%s: n = %d > max = %d", s, n , max);
}

void rbc_rnd_gen(RbcRnd *d, int n) {
    assert_n(n, d->max, "rbc::rnd::gen");
    rnd_api::GenerateNormal(d->g, d->r, n);
}

float rbc_rnd_get_hst(const RbcRnd *d, int i) {
    float x;
    float *r;
    assert_n(i, d->max, "rbc::rnd::get_hst");
    r = d->r;
    cD2H(&x, &r[i], 1);
    return x;
}
