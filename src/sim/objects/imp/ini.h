static void gen_name_mesh_dir(const char *name, char *mesh_dir) {
    strcpy(mesh_dir, "ply/");
    strcat(mesh_dir, name);
}

static void ini_mbr_distr(bool ids, int nv, MPI_Comm comm, int3 L, /**/ MbrDistr *d) {
    UC(drbc_pack_ini(ids, L, MAX_CELL_NUM, nv, /**/ &d->p));
    UC(drbc_comm_ini(comm, /**/ &d->c));
    UC(drbc_unpack_ini(ids, L, MAX_CELL_NUM, nv, /**/ &d->u));
}

static void ini_rig_distr(int nv, MPI_Comm comm, int3 L, /**/ RigDistr *d) {
    UC(drig_pack_ini(L, MAX_SOLIDS, nv, /**/ &d->p));
    UC(drig_comm_ini(comm, /**/ &d->c));
    UC(drig_unpack_ini(L,MAX_SOLIDS, nv, /**/ &d->u));
}

static void ini_mesh_exch(int3 L, int nv, int max_m, MPI_Comm comm, /**/ MeshExch **me) {
    MeshExch *e;
    EMALLOC(1, me);
    e = *me;
    UC(emesh_pack_ini(L, nv, max_m, /**/ &e->p));
    UC(emesh_comm_ini(comm, /**/ &e->c));
    UC(emesh_unpack_ini(L, nv, max_m, /**/ &e->u));
}

static void ini_mesh_mom_exch(int nt, int max_m, MPI_Comm comm, /**/ MeshMomExch **mme) {
    MeshMomExch *e;
    EMALLOC(1, mme);
    e = *mme;
    UC(emesh_packm_ini(nt, max_m, /**/ &e->p));
    UC(emesh_commm_ini(comm, /**/ &e->c));
    UC(emesh_unpackm_ini(nt, max_m, /**/ &e->u));
}

static void ini_bbdata(int nt, int max_m, MPI_Comm cart, /**/ BounceBackData **bbdata) {
    BounceBackData *bb;
    EMALLOC(1, bbdata);
    bb = *bbdata;
    UC(ini_mesh_mom_exch(nt, max_m, cart, &bb->e));
    Dalloc(&bb->mm, max_m * nt);
}

static void ini_colorer(int nv, int max_m, /**/ Colorer **col) {
    Colorer *c;
    EMALLOC(1, col);
    c = *col;
    Dalloc(&c->pp_mesh, nv * max_m);
    Dalloc(&c->lo, max_m);
    Dalloc(&c->hi, max_m);
}

static void read_params(const Config *cfg, const char *ns, PairParams **par) {
    PairParams *p;
    UC(pair_ini(par));
    p = *par;
    UC(pair_set_conf(cfg, ns, p));
}

static void ini_params(const Config *cfg, const char *name, const char *pair, PairParams **par) {
    const char *ns;
    UC(conf_lookup_string_ns(cfg, name, pair, &ns));

    if (same_str(ns, "none")) *par = NULL;
    else UC(read_params(cfg, ns, par));
}

static void ini_repulsion_params(const Config *cfg, const char *name, WallRepulsePrm **par) {
    const char *ns;
    UC(conf_lookup_string_ns(cfg, name, "repulsion", &ns));

    if (same_str(ns, "none")) *par = NULL;
    else UC(wall_repulse_prm_ini_conf(cfg, ns, par));
}

static void read_mesh(const char *filename, MeshRead **mesh) {
    const char *ext;
    ext = get_filename_ext(filename);

    if      (same_str(ext, "off"))
        UC(mesh_read_ini_off(filename, mesh));
    else if (same_str(ext, "ply"))
        UC(mesh_read_ini_ply(filename, mesh));
    else
        ERR("%s : unrecognised extension <%s>\n", filename, ext);
}

static void ini_obj_common(const Config *cfg, const OptObj *opt, MPI_Comm cart, Obj *obj) {
    char mesh_dir[FILENAME_MAX];
    strcpy(obj->name, opt->name);
    strcpy(obj->ic_file, opt->ic_file);
    gen_name_mesh_dir(obj->name, mesh_dir);

    UC(read_mesh(opt->templ_file, &obj->mesh));
    UC(mesh_write_ini_from_mesh(cart, opt->shifttype, obj->mesh, mesh_dir, /**/ &obj->mesh_write));

    obj->mass = opt->mass;

    UC(ini_params(cfg, obj->name, "fsi", &obj->fsi));
    UC(ini_params(cfg, obj->name, "adhesion", &obj->adhesion));
    UC(ini_repulsion_params(cfg, obj->name, &obj->wall_rep_prm));
}

static void ini_mbr(const Config *cfg, const OptMbr *opt, MPI_Comm cart, int3 L,
                    bool recolor, /**/ Mbr **membrane) {
    int nv, max_m;
    Mbr *m;
    EMALLOC(1, membrane);
    m = *membrane;
    
    max_m = MAX_CELL_NUM;
    
    m->com       = NULL;
    m->stretch   = NULL;
    m->colorer   = NULL;
    m->mesh_exch = NULL;

    UC(ini_obj_common(cfg, opt, cart, m));

    nv = mesh_read_get_nv(m->mesh);
    
    Dalloc(&m->ff,      max_m * nv);
    Dalloc(&m->ff_fast, max_m * nv);
    
    UC(triangles_ini(m->mesh, /**/ &m->tri));
    UC(rbc_ini(max_m, opt->ids, m->mesh, &m->q));
    UC(ini_mbr_distr(opt->ids, nv, cart, L, /**/ &m->d));

    if (opt->dump_com) UC(rbc_com_ini(nv, max_m, /**/ &m->com));
    if (opt->stretch)  UC(rbc_stretch_ini(opt->stretch_file, nv, /**/ &m->stretch));

    UC(rbc_params_ini(&m->params));
    UC(rbc_params_set_conf(cfg, m->name, m->params));

    UC(rbc_force_ini(m->mesh, /**/ &m->force));
    UC(rbc_force_set_conf(m->mesh, cfg, m->name, m->force));

    if (recolor) UC(ini_mesh_exch(L, nv, max_m, cart, /**/ &m->mesh_exch));
    if (recolor) UC(ini_colorer(nv, max_m, /**/ &m->colorer));
}

static void ini_rig(const Config *cfg, const OptRig *opt, MPI_Comm cart, int maxp, int3 L, /**/ Rig **rigid) {
    Rig *r;
    long max_m = MAX_SOLIDS;
    int nv;
    EMALLOC(1, rigid);
    r = *rigid;

    r->bbdata    = NULL;
    r->mesh_exch = NULL;

    UC(ini_obj_common(cfg, opt, cart, r));
    
    UC(rig_ini(max_m, maxp, r->mesh, &r->q));
    
    Dalloc(&r->ff, maxp);

    nv = r->q.nv;

    UC(ini_rig_distr(nv, cart, L, /**/ &r->d));

    UC(rig_pininfo_ini(&r->pininfo));
    UC(rig_pininfo_set_conf(cfg, r->name, r->pininfo));

    if (opt->bounce) UC(ini_mesh_exch(L, nv, max_m, cart, /**/ &r->mesh_exch));
    if (opt->bounce) UC(ini_bbdata(r->q.nt, max_m, cart, /**/ &r->bbdata));
}

static void ini_dump(long maxp, Dump **dump) {
    Dump *d;
    EMALLOC(1, dump);
    d = *dump;
    EMALLOC(maxp, &d->pp);
    d->id = d->id_diag = 0;
}

static bool need_bb(int n, const OptRig *r) {
    int i;
    for (i = 0; i < n; ++i) if (r[i].bounce) return true;
    return false;
}

void objects_ini(const Config *cfg, const Opt *opt, MPI_Comm cart, const Coords *coords, int maxp, Objects **objects) {
    Objects *obj;
    int3 L;
    bool recolor = opt->flu.colors;
    int i;
    EMALLOC(1, objects);
    obj = *objects;
    obj->opt = *opt;
    obj->active = false;

    MC(m::Comm_dup(cart, &obj->cart));
    L = subdomain(coords);
    UC(coords_ini(cart, L.x, L.y, L.z, &obj->coords));

    obj->bb  = NULL;

    obj->nmbr = opt->nmbr;
    obj->nrig = opt->nrig;

    if (obj->nmbr) EMALLOC(obj->nmbr, &obj->mbr);
    if (obj->nrig) EMALLOC(obj->nrig, &obj->rig);

    for (i = 0; i < obj->nmbr; ++i) UC(ini_mbr(cfg, &opt->mbr[i], cart, L, recolor, &obj->mbr[i]));
    for (i = 0; i < obj->nrig; ++i) UC(ini_rig(cfg, &opt->rig[i], cart, maxp, L, &obj->rig[i]));

    if (need_bb(opt->nrig, opt->rig)) UC(meshbb_ini(maxp, /**/ &obj->bb));
        
    UC(ini_dump(maxp, &obj->dump));
}
