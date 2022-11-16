static void ini_obj_exch(MPI_Comm comm, int3 L, /**/ ObjExch **oe) {
    ObjExch *e;
    int maxpsolid = MAX_PSOLID_NUM;
    EMALLOC(1, oe);
    e = *oe;

    UC(eobj_pack_ini   (L, MAX_OBJ_TYPES, MAX_OBJ_DENSITY, maxpsolid, /**/ &e->p));
    UC(eobj_comm_ini   (comm,                                         /**/ &e->c));
    UC(eobj_unpack_ini (L, MAX_OBJ_TYPES, MAX_OBJ_DENSITY, maxpsolid, /**/ &e->u));

    UC(eobj_packf_ini  (L, MAX_OBJ_DENSITY, maxpsolid, /**/ &e->pf));
    UC(eobj_commf_ini  (comm,                          /**/ &e->cf));
    UC(eobj_unpackf_ini(L, MAX_OBJ_DENSITY, maxpsolid, /**/ &e->uf));
}

void obj_inter_ini(const Config *cfg, const Opt *opt, MPI_Comm cart, float dt, int maxp, /**/ ObjInter **oi) {
    int rank;
    ObjInter *o;
    int3 L = opt->params.L;
    EMALLOC(1, oi);
    o = *oi;
    
    MC(m::Comm_rank(cart, &rank));
    UC(ini_obj_exch(cart, L, &o->e));

    o->cnt = NULL;
    o->fsi = NULL;

    int nobj = opt->nmbr + opt->nrig;
    UC(cnt_ini(maxp, rank, L, nobj, /**/ &o->cnt));
    UC(fsi_ini(rank, L, /**/ &o->fsi));
}


static void fin_obj_exch(/**/ ObjExch *e) {
    UC(eobj_pack_fin   (/**/ e->p));
    UC(eobj_comm_fin   (/**/ e->c));
    UC(eobj_unpack_fin (/**/ e->u));

    UC(eobj_packf_fin  (/**/ e->pf));
    UC(eobj_commf_fin  (/**/ e->cf));
    UC(eobj_unpackf_fin(/**/ e->uf));
    EFREE(e);
}

void obj_inter_fin(ObjInter *o) {
    UC(fin_obj_exch(o->e));
    if (o->cnt) UC(cnt_fin(o->cnt));
    if (o->fsi) UC(fsi_fin(o->fsi));
}
