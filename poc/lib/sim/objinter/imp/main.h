static void fill_wrappers(PFarrays *obj, int *nwrappers, PaWrap *pw, FoWrap *fw) {
    long i, nw, n;
    PaArray p;
    FoArray f;

    (*nwrappers) = nw = pfarrays_size(obj);

    if (nw > MAX_OBJ_TYPES)
        ERR("Too many objects in wrappers : %d/%d", nw, MAX_OBJ_TYPES);

    for (i = 0; i < nw; ++i, ++pw, ++fw) {
        UC(pfarrays_get(i, obj, &n, &p, &f));
        pw->n = fw->n = n;
        pw->pp = (const Particle*) p.pp;
        fw->ff = (Force*) f.ff;
    }
}

static void bind_solvent(PFarray *flu, const int *starts, Fsi *fsi) {
    UC(fsi_bind_solvent(flu->p, (Force*) flu->f.ff, flu->n, starts, /**/ fsi));
}

static void forces_cnt(ObjInter *oi, int nw, const PairParams *prms[], PaWrap *pw, FoWrap *fw) {
    cnt_build_cells(  nw, prms, pw, /**/ oi->cnt);
    cnt_bulk(oi->cnt, nw, prms, pw, fw);
}

static void forces_fsi(ObjInter *oi, int nw, const PairParams *prms[], PaWrap *pw, FoWrap *fw) {
    fsi_bulk(oi->fsi, nw, prms, pw, fw);
}

static bool has_fsi_work(int nw, const PairParams **prms) {
    for (int i = 0; i < nw; ++i)
        if (prms[i]) return true;
    return false;
}

static bool has_work(const ObjInter *o, int nw, const PairParams **fsi_prms) {
    return o->cnt || has_fsi_work(nw, fsi_prms);
}

void obj_inter_forces(ObjInter *oi, const PairParams **fsi_prms, const PairParams **cnt_prms, PFarray *flu, const int *flu_start, PFarrays *obj) {
    PaWrap pw[MAX_OBJ_TYPES];
    FoWrap fw[MAX_OBJ_TYPES];
    int nw = 0;
    ObjExch *e = oi->e;
    int all_counts[26 * MAX_OBJ_TYPES] = {0};
    
    UC(fill_wrappers(obj, /**/ &nw, pw, fw));
    if (!nw) return;
    if (!has_work(oi, nw, fsi_prms)) return;

    /* Prepare and send the data */
    
    UC(eobj_build_map(nw, pw, /**/ e->p));
    UC(eobj_pack(nw, pw, /**/ e->p));
    UC(eobj_download(nw, /**/ e->p));

    UC(eobj_post_send(e->p, e->c));
    UC(eobj_post_recv(e->c, e->u));

    /* bulk interactions */
    
    if (oi->fsi) UC(bind_solvent(flu, flu_start, oi->fsi));

    if (oi->fsi) UC(forces_fsi(oi, nw, fsi_prms, pw, fw));
    if (oi->cnt) UC(forces_cnt(oi, nw, cnt_prms, pw, fw));

    /* recv data and halo interactions  */

    UC(eobj_wait_send(e->c));
    UC(eobj_wait_recv(e->c, e->u));

    eobj_get_all_counts(nw, e->u, /**/ all_counts);
    Pap26 hpp = eobj_upload_shift(e->u);
    Fop26 hff = eobj_reini_ff(e->u, e->pf);

    if (oi->fsi) UC(fsi_halo(oi->fsi, nw, fsi_prms,         hpp, hff, all_counts));
    if (oi->cnt) UC(cnt_halo(oi->cnt, nw, cnt_prms, pw, fw, hpp, hff, all_counts));

    /* send the forces back */ 
    
    UC(eobj_download_ff(e->pf));

    UC(eobj_post_send_ff(e->pf, e->cf));
    UC(eobj_post_recv_ff(e->cf, e->uf));

    UC(eobj_wait_send_ff(e->cf));
    UC(eobj_wait_recv_ff(e->cf, e->uf));

    UC(eobj_unpack_ff(e->uf, e->p, nw, /**/ fw));
}
