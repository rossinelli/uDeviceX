void eflu_pack(const PaArray *parray, /**/ EFluPack *p) {
    int nc, i, cap;
    int26 cc;
    Pap26 fpp;
    intp26 fcc;
    const int *bss, *bcc, *fss;

    CC(d::MemsetAsync(p->counts_dev, 0, NFRAGS * sizeof(int)));
    
    get_cell_num(p->L, /**/ cc.d);

    bag2Sarray(*p->dpp, /**/ &fpp);
    if (parray->colors) bag2Sarray(*p->dcc, /**/ &fcc);
        
    for (i = 0; i < 26; ++i) {
        nc = cc.d[i];
        bss = p->bss.d[i];
        bcc = p->bcc.d[i];
        fss = p->fss.d[i];
        cap = p->cap.d[i];
        
        KL( eflu_dev::collect_particles,
            ((nc+1) / 2, 32),
            (i, nc, (const Particle*) parray->pp, bss, bcc, fss, cap, /**/ p->bii.d[i], fpp.d[i], p->counts_dev));

        if (parray->colors)
            KL( eflu_dev::collect_colors,
                ((nc+1) / 2, 32),
                (i, nc, parray->cc, bss, bcc, fss, cap, /**/ fcc.d[i]));
    }
}

static void copy(int, const int counts[], const dBags *d, /**/ hBags *h) {
    int i, c;
    size_t sz;
    
    for (i = 0; i < NFRAGS; ++i) {
        c = counts[i];
        sz = c * h->bsize;
        if (c)
            CC(d::MemcpyAsync(h->data[i], d->data[i], sz, D2H));
    }
}

void eflu_download_data(EFluPack *p) {
    int counts[NFRAGS];
    size_t sz = sizeof(counts);

    CC(d::MemcpyAsync(counts, p->counts_dev, sz, D2H));
    dSync(); /* wait for counts memcpy */

    copy(NFRAGS, counts, p->dpp, /**/ p->hpp);
    if (p->hcc)
        copy(NFRAGS, counts, p->dcc, /**/ p->hcc);

    memcpy(p->hpp->counts, counts, sz);
    if (p->hcc)
        memcpy(p->hcc->counts, counts, sz);

    dSync(); /* wait for copy */
}
