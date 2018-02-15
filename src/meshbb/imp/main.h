void meshbb_ini(int maxpp, /**/ MeshBB **meshbb) {
    MeshBB *mbb;
    UC(emalloc(sizeof(MeshBB), (void**) meshbb));
    mbb = *meshbb;
    CC(d::Malloc((void**) &mbb->ncols,   maxpp * sizeof(int)));
    CC(d::Malloc((void**) &mbb->datacol, maxpp * MAX_COL * sizeof(float4)));
    CC(d::Malloc((void**) &mbb->idcol,   maxpp * MAX_COL * sizeof(int)));
}

void meshbb_fin(/**/ MeshBB *mbb) {
    CC(d::Free(mbb->ncols));
    CC(d::Free(mbb->datacol));
    CC(d::Free(mbb->idcol));
    UC(efree(mbb));
}

void meshbb_reini(int n, /**/ MeshBB *mbb) {
    CC(d::MemsetAsync(mbb->ncols, 0, n * sizeof(int)));
}

void meshbb_select_collisions(float dt, int n, /**/ MeshBB *mbb) {
    KL(meshbb_dev::select_collisions, (k_cnf(n)), (dt, n, /**/ mbb->ncols, mbb->datacol, mbb->idcol));
}

void meshbb_bounce(float dt,
                   int n, const MeshBB *mbb, const Force *ff, int nt, int nv, const int4 *tt, const Particle *i_pp,
                   /**/ Particle *pp, Momentum *mm) {
    KL(meshbb_dev::perform_collisions, (k_cnf(n)),
       (dt, n, mbb->ncols, mbb->datacol, mbb->idcol, ff, nt, nv, tt, i_pp, /**/ pp, mm));
}

void meshbb_collect_rig_momentum(float dt,
                                 int ns, int nt, int nv, const int4 *tt, const Particle *pp, const Momentum *mm, /**/ Solid *ss) {
    KL(meshbb_dev::collect_rig_mom, (k_cnf(ns * nt)), (dt, ns, nt, nv, tt, pp, mm, /**/ ss));
}

void meshbb_collect_rbc_momentum(float dt, int nc, int nt, int nv, const int4 *tt, const Particle *pp, const Momentum *mm, /**/ Force *ff) {
    KL(meshbb_dev::collect_rbc_mom, (k_cnf(nc * nt)), (dt, nc, nt, nv, tt, pp, mm, /**/ ff));
}
