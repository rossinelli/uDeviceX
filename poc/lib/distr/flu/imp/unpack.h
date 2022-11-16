static int scan_hst(const int n, const int *counts, int27 *starts) {
    int i, s;
    starts->d[0] = 0;
    for (i = 0, s = 0; i < n; ++i)
        starts->d[i + 1] = (s += counts[i]);
    return s;
}

template <typename T>
static void unpack(const hBags *bags, int27 starts, /**/ T *buf) {
    int c, s, i;
    size_t sz, bs = bags->bsize;

    assert(bs == sizeof(T));
    
    for (i = 0; i < NFRAGS; ++i) {
        c = bags->counts[i];
        sz = c * bs;
        s = starts.d[i];
        if (c)
            CC(d::MemcpyAsync(buf + s, bags->data[i], sz, H2D));
    }
}

static int unpack_pp(int3 L, const hBags *bags, /**/ Particle *pp) {
    int nhalo;
    int27 starts;

    nhalo = scan_hst(NFRAGS, bags->counts, &starts);    
    unpack(bags, starts, /**/ pp);

    dcommon_shift_halo(L, nhalo, starts, /**/ pp);
    
    return nhalo;
}

static int unpack_ii(const hBags *bags, /**/ int *ii) {
    int nhalo;
    int27 starts;

    nhalo = scan_hst(NFRAGS, bags->counts, &starts);    
    unpack(bags, starts, /**/ ii);
    
    return nhalo;
}

static void unpack_pp(/**/ DFluUnpack *u) {
    u->nhalo = unpack_pp(u->L, u->hpp, /**/ u->ppre);
}

static void unpack_ii(/**/ DFluUnpack *u) {
    if (u->hii) unpack_ii(u->hii, /**/ u->iire);
}

static void unpack_cc(/**/ DFluUnpack *u) {
    if (u->hcc) unpack_ii(u->hcc, /**/ u->ccre);
}

void dflu_unpack(/**/ DFluUnpack *u) {
    unpack_pp(/**/ u);
    unpack_ii(/**/ u);
    unpack_cc(/**/ u);
}
