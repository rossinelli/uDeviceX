static void upload_and_shift_pp(int n, int i, int3 L, const data_t *data, Particle *pp) {
    if (n == 0) return;
    size_t sz = n * sizeof(Particle);
    CC(d::MemcpyAsync(pp, data, sz, H2D));
    ecommon_shift_pp_one_frag(L, n, i, /**/ pp);
}

void emesh_unpack_pp(int nv, const EMeshUnpack *u, /**/ int *nmhalo, Particle *pp) {
    int i, nm, n, s = 0, nmtot = 0;
    
    for (i = 0; i < NFRAGS; ++i) {
        nm = u->hpp->counts[i];
        n  = nm * nv; 
        upload_and_shift_pp(n, i, u->L, u->hpp->data[i], pp + s);
        s += n;
        nmtot += nm;
    }
    *nmhalo = nmtot;
}

static void upload_and_shift_rrcp(int n, int i, int3 L, const data_t *data, Positioncp *rr) {
    if (n == 0) return;
    size_t sz = n * sizeof(Positioncp);
    CC(d::MemcpyAsync(rr, data, sz, H2D));
    ecommon_shift_rrcp_one_frag(L, n, i, /**/ rr);
}

void emesh_unpack_rrcp(int nv, const EMeshUnpack *u, /**/ int *nmhalo, Positioncp *rr) {
    int i, nm, n, s = 0, nmtot = 0;
    
    for (i = 0; i < NFRAGS; ++i) {
        nm = u->hpp->counts[i];
        n  = nm * nv; 
        upload_and_shift_rrcp(n, i, u->L, u->hpp->data[i], rr + s);
        s += n;
        nmtot += nm;
    }
    *nmhalo = nmtot;
}

void emesh_get_num_frag_mesh(const EMeshUnpack *u, /**/ int cc[NFRAGS]) {
    memcpy(cc, u->hpp->counts, NFRAGS * sizeof(int));
}

/* momentum */
static void upload_bags(const hBags *h, dBags *d) {
    int i, c;
    data_t *src, *dst;
    size_t sz;
    for (i = 0; i < NFRAGS; ++i) {
        c = h->counts[i];
        sz = c * h->bsize;
        src = h->data[i];
        dst = d->data[i];
        if (sz)
            CC(d::MemcpyAsync(dst, src, sz, H2D));
    }
}

void emesh_upload(EMeshUnpackM *u) {
    upload_bags(u->hmm, u->dmm);
    upload_bags(u->hii, u->dii);
}

static int get_fragstarts(int nfrags, const int cc[], /**/ int *starts) {
    int i, s;
    starts[0] = s = 0;
    for (i = 0; i < nfrags; ++i)
        starts[i+1] = (s += cc[i]);
    return s;
}

void emesh_unpack_mom(int nt, const EMeshPack *p, const EMeshUnpackM *u, /**/ Momentum *mm) {
    intp26 wrapii;
    Mop26 wrapmm;
    int27 fragstarts;
    int n;
    
    bag2Sarray(*u->dmm, &wrapmm);
    bag2Sarray(*u->dii, &wrapii);

    n = get_fragstarts(NFRAGS, u->hii->counts, /**/ fragstarts.d);
    
    KL(emesh_dev::unpack_mom, (k_cnf(n)), (nt, fragstarts, wrapii, wrapmm, p->map, /**/ mm));
}
