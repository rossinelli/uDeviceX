static void pack_mesh_pp(int nv, const Particle *pp, EMap map, /**/ Pap26 buf) {
    KL(emesh_dev::pack_mesh, (14 * 16, 128), (nv, pp, map, /**/ buf));
}

void emesh_pack_pp(int nv, const Particle *pp, /**/ EMeshPack *p) {
    Pap26 wrap;
    bag2Sarray(*p->dpp, &wrap);
    pack_mesh_pp(nv, pp, p->map, /**/ wrap);
}

static void pack_mesh_rrcp(int nv, const Positioncp *rr, EMap map, /**/ Pocpp26 buf) {
    KL(emesh_dev::pack_mesh, (14 * 16, 128), (nv, rr, map, /**/ buf));
}

void emesh_pack_rrcp(int nv, const Positioncp *rr, /**/ EMeshPack *p) {
    Pocpp26 wrap;
    bag2Sarray(*p->dpp, &wrap);
    pack_mesh_rrcp(nv, rr, p->map, /**/ wrap);
}

void emesh_download(EMeshPack *p) {
    int nw = 1;
    emap_download_tot_counts(nw, NFRAGS, p->map, /**/ p->hpp->counts);
}

static void reini_map(int nm, /**/ MMap *m) {
    if (nm) CC(d::MemsetAsync(m->cc, 0, nm * sizeof(int)));
}

static void compress_mom(int nt, int nm, const Momentum *mm, /**/ MMap *m, int *ids, Momentum *mmc) {
    UC(reini_map(nm, /**/ m));
    KL(emesh_dev::subindex_compress, (k_cnf(nt * nm)), (nt, nm, mm, /**/ m->cc, m->subids));

    enum {NWRP=4, WRPSZ=32};
    enum {BLCK=1, THRD=NWRP*WRPSZ};
    KL(emesh_dev::block_scan<NWRP>, (BLCK, THRD), (nm, m->cc, /**/ m->ss));

    KL(emesh_dev::compress, (k_cnf(nt * nm)), (nt, nm, mm, m->ss, m->subids, /**/ ids, mmc));
}

static void pack_mom(int nt, const int counts[NFRAGS], const Momentum *mm,
                     /**/ MMap maps[NFRAGS], int *ibuf[NFRAGS], Momentum *mbuf[NFRAGS]) {
    int i, s, c;

    for (i = s = 0; i < NFRAGS; ++i) {
        c = counts[i];
        UC(compress_mom(nt, c, mm + nt * s, /**/ maps + i, ibuf[i], mbuf[i]));
    }
}

void emesh_packM(int nt, const int counts[NFRAGS], const Momentum *mm, /**/ EMeshPackM *p) {
    pack_mom(nt, counts, mm, /**/ p->maps, (int **) p->dii->data, (Momentum **) p->dmm->data);
}

void emesh_downloadM(const int counts[NFRAGS], EMeshPackM *p) {
    MMap26 mm;
    int26 ii;
    size_t sz;
    sz = NFRAGS * sizeof(int);
    
    memcpy(ii.d, counts, sz);
    memcpy(mm.d, p->maps, NFRAGS * sizeof(MMap));

    KL(emesh_dev::collect_counts, (32, 1), (ii, mm, /**/ p->ccdev));

    dSync();

    memcpy(p->hmm->counts, p->cchst, sz);
    memcpy(p->hii->counts, p->cchst, sz);
}

