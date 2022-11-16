int26 eobj_get_counts(EObjUnpack *u) {
    int26 cc;
    memcpy(cc.d, u->hpp->counts, NFRAGS * sizeof(int));
    return cc;
}

void  eobj_get_all_counts(int nw, EObjUnpack *u, int *all_counts) {
    hBags *cc = u->hcc;
    int i, j, *ci;
    for (i = 0; i < NFRAGS; ++i) {
        ci = (int*) cc->data[i];
        for (j = 0; j < nw; ++j)
            all_counts[j * NFRAGS + i] = ci[j];
    }
}

static void upload(int nfrags, const hBags *h, /**/ dBags *d) {
    int i, c;
    size_t sz;
    data_t *src, *dst;

    for (i = 0; i < nfrags; ++i) {
        c = h->counts[i];
        if (c) {
            sz  = h->bsize * c;
            dst = d->data[i];
            src = h->data[i];
            CC(d::MemcpyAsync(dst, src, sz, H2D));
        }
    }    
}

static void shift_pp(int3 L, int nfrags, const int counts[], /**/ dBags d) {
    int i, n;
    Particle *pp;

    for (i = 0; i < nfrags; ++i) {
        n = counts[i];
        if (n) {
            pp = (Particle *) d.data[i];
            ecommon_shift_pp_one_frag(L, n, i, /**/ pp);
        }
    }    
}

Pap26 eobj_upload_shift(EObjUnpack *u) {
    upload(NFRAGS, u->hpp, /**/ &u->dpp);
    shift_pp(u->L, NFRAGS, u->hpp->counts, /**/ u->dpp);
    Pap26 wrap;
    bag2Sarray(u->dpp, &wrap);
    return wrap;
}

static void unpack_ff(int nfrags, Fop26 ff, EMap map, int nw, /**/ FoWrap *ww) {
    int i, stride;
    stride = nfrags + 1;
    const FoWrap *w;
    PackHelper ph;
    
    for (i = 0; i < nw; ++i) {
        w = ww + i;
        ph.starts  = map.starts  + i * stride;
        ph.offsets = map.offsets + i * stride;
        memcpy(ph.indices, map.ids, nfrags * sizeof(int*));

        KL(eobj_dev::unpack_ff, (14 * 16, 128), (ff, ph, /**/ w->ff));
    }
}

void eobj_unpack_ff(EObjUnpackF *u, const EObjPack *p, int nw, /**/ FoWrap *ww) {
    Fop26 wrap;
    upload(NFRAGS, &u->hff, /**/ &u->dff);
    bag2Sarray(u->dff, &wrap);
    unpack_ff(NFRAGS, wrap, p->map, nw, /**/ ww);
}
