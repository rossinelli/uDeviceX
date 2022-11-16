static void freeze0(MPI_Comm cart, int numdensity, int3 L, int maxn, const Sdf *qsdf, /*io*/ int *n, Particle *pp, /*o*/ int *w_n, Particle *dev, /*w*/ Particle *hst) {
    sdf_bulk_wall(qsdf, /*io*/ n, pp, /*o*/ w_n, hst); /* sort into bulk-frozen */
    msg_print("before exch: bulk/frozen : %d/%d", *n, *w_n);
    UC(wall_exch_pp(cart, numdensity, L, maxn, /*io*/ hst, w_n));
    cH2D(dev, hst, *w_n);
    msg_print("after  exch: bulk/frozen : %d/%d", *n, *w_n);
}

static void freeze(MPI_Comm cart, int numdensity, int3 L, int maxn, const Sdf *qsdf, /*io*/ int *n, Particle *pp, /*o*/ int *w_n, Particle *dev) {
    Particle *hst;
    EMALLOC(maxn, &hst);
    UC(freeze0(cart, numdensity, L, maxn, qsdf, /*io*/ n, pp, /*o*/ w_n, dev, /*w*/ hst));
    EFREE(hst);
}

static void gen_quants(MPI_Comm cart, int numdensity, int3 L, int maxn, const Sdf *qsdf, /**/ int *o_n, Particle *o_pp, int *w_n, float4 **w_pp) {
    Particle *frozen;
    Dalloc(&frozen, maxn);
    UC(freeze(cart, numdensity, L, maxn, qsdf, o_n, o_pp, w_n, frozen));
    msg_print("consolidating wall");
    Dalloc(w_pp, *w_n);
    KL(wall_dev::particle2float4, (k_cnf(*w_n)), (frozen, *w_n, /**/ *w_pp));
    
    Dfree(frozen);
    dSync();
}

void wall_gen_quants(MPI_Comm cart, int numdensity, int maxn, const Sdf *sdf, /* io */ int *o_n, Particle *o_pp, /**/ WallQuants *q) {
    int3 L = q->L;
    UC(gen_quants(cart, numdensity, L, maxn, sdf, o_n, o_pp, &q->n, &q->pp));
}

static void build_cells(const int n, float4 *pp4, Clist *cells, ClistMap *mcells) {
    if (n == 0) return;

    Particle *pp, *pp0;
    Dalloc(&pp,  n);
    Dalloc(&pp0, n);

    KL(wall_dev::float42particle, (k_cnf(n)), (pp4, n, /**/ pp));
    UC(clist_build(n, n, pp, /**/ pp0, cells, mcells));
    KL(wall_dev::particle2float4, (k_cnf(n)), (pp0, n, /**/ pp4));

    Dfree(pp);
    Dfree(pp0);
}

static void gen_ticket(const int w_n, float4 *w_pp, Clist *cells, Texo<int> *texstart, Texo<float4> *texpp) {
    ClistMap *mcells;
    UC(clist_ini_map(w_n, 1, cells, /**/ &mcells));
    UC(build_cells(w_n, /**/ w_pp, cells, mcells));
    
    UC(texo_setup(clists_get_n(cells), (int*) clists_get_ss(cells), texstart));
    UC(texo_setup(w_n, w_pp, texpp));
    UC(clist_fin_map(mcells));
}

void wall_gen_ticket(const WallQuants *q, WallTicket *t) {
    UC(gen_ticket(q->n, q->pp, t->cells, &t->texstart, &t->texpp));
}
