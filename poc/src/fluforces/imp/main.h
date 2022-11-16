void fluforces_bulk_prepare(int n, const PaArray *a, /**/ FluForcesBulk *b) {
    if (n == 0) return;
    convert_pp2f4_pos(n, a->pp, /**/ b->zipped_pp);
    if (a->colors)
        b->colors = a->cc;
    else
        b->colors = NULL;
}

void fluforces_bulk_apply(const PairParams *par, int n, const FluForcesBulk *b, const int *start, const int *count,
                          /**/ const FoArray *farray) {
    BPaArray a;
    if (n == 0) return;

    flocal_push_pp(b->zipped_pp, &a);
    if (b->colors)
        flocal_push_cc(b->colors, &a);

    UC(flocal_apply(par, b->L, n, a, start, b->rnd, /**/ farray));
}


void fluforces_halo_prepare(flu::LFrag26 lfrags, flu::RFrag26 rfrags, /**/ FluForcesHalo *h) {
    h->lfrags = lfrags;
    h->rfrags = rfrags;

    for (int i = 0; i < 26; ++i) {
        h->rndfrags.d[i] = {
            .seed = rnd_get(h->trunks[i]),
            .mask = h->masks[i]
        };
    }
}

void fluforces_halo_apply(const PairParams *par, const FluForcesHalo *h, /**/ const FoArray *farray) {
    fhalo_apply(par, h->L, h->lfrags, h->rfrags, h->rndfrags, farray);
}
