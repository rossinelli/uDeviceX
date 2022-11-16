static bool is_stress_free(const RbcForce *f) {
    return f->stype == RBC_SFREE;
}

static bool is_rnd(const RbcForce *f) {
    return f->rtype == RBC_RND1;
}

void rbc_force_ini(const MeshRead *cell, RbcForce **pq) {
    RbcForce *q;
    int md, nt, nv;
    const int4 *tt;
    EMALLOC(1, &q);
    nv = mesh_read_get_nv(cell);
    nt = mesh_read_get_nt(cell);
    md = mesh_read_get_md(cell);
    tt = mesh_read_get_tri(cell);
    
    UC(adj_ini(md, nt, nv, tt, /**/ &q->adj));
    UC(adj_view_ini(q->adj, /**/ &q->adj_v));

    *pq = q;
}

static void fin_rnd(RbcRnd *rnd) {
    rbc_rnd_fin(rnd);
}

static void fin_stress(RbcForce *f) {
    if (is_stress_free(f)) {
        StressFree_v v = f->sinfo.sfree;
        Dfree(v.ll);
        Dfree(v.aa);
    }
}

void rbc_force_fin(RbcForce *q) {
    if (is_rnd(q)) {
        Rnd1_v v = q->rinfo.rnd1;
        Dfree(v.anti);
        UC(fin_rnd(q->rnd));
    }        
    UC(fin_stress(q));
    UC(adj_fin(q->adj));
    UC(adj_view_fin(q->adj_v));
    UC(bending_fin(q->bending));
    EFREE(q);
}

void rbc_force_set_stressful(int nt, float totArea, /**/ RbcForce *f) {
    StressFul_v v;
    float a0 = totArea / nt;

    v.a0 = a0;
    v.l0 = sqrt(a0 * 4.0 / sqrt(3.0));
    
    f->stype = RBC_SFUL;
    f->sinfo.sful = v;
}

void rbc_force_set_stressfree(const char *fname, /**/ RbcForce *f) {
    StressFree_v v;
    MeshRead *cell;
    const Adj *adj;
    RbcShape *shape;
    const float *rr;
    float *ll_hst, *aa_hst;
    int n;

    adj = f->adj;    
    UC(mesh_read_ini_off(fname, &cell));
    rr = mesh_read_get_vert(cell);

    UC(rbc_shape_ini(adj, rr, /**/ &shape));

    n = adj_get_max(adj);

    rbc_shape_edg(shape, &ll_hst);
    rbc_shape_area(shape, &aa_hst);

    Dalloc(&v.ll, n);
    Dalloc(&v.aa, n);
    
    cH2D(v.ll, ll_hst, n);
    cH2D(v.aa, aa_hst, n);
    
    UC(rbc_shape_fin(shape));
    UC(mesh_read_fin(cell));

    f->stype = RBC_SFREE;
    f->sinfo.sfree = v;
}

void rbc_force_set_rnd0(RbcForce *f) {
    Rnd0_v rnd0;
    f->rtype = RBC_RND0;
    f->rinfo.rnd0 = rnd0;
}

void rbc_force_set_rnd1(int seed, RbcForce *f) {
    Rnd1_v rnd1;
    int n;
    int *hst;
    Adj *adj = f->adj;

    n = adj_get_max(adj);
    Dalloc(&rnd1.anti, n);
    
    EMALLOC(n, &hst);
    adj_get_anti(adj, /**/ hst);
    cH2D(rnd1.anti, hst, n);
    EFREE(hst);

    UC(rbc_rnd_ini(n * MAX_CELL_NUM, seed, &f->rnd));
    
    f->rtype = RBC_RND1;
    f->rinfo.rnd1 = rnd1;
}

void rbc_force_set_bending(const MeshRead *cell, const char *type, RbcForce *q) {
    if (same_str(type, "kantor"))
        UC(bending_kantor_ini(cell, /**/ &q->bending));
    else if (same_str(type, "juelicher"))
        UC(bending_juelicher_ini(cell, /**/ &q->bending));
    else if (same_str(type, "none"))
        UC(bending_none_ini(cell, /**/ &q->bending));
    else
        ERR("unknown bending type '%s'", type);
    msg_print("bending: %s", type);
}
