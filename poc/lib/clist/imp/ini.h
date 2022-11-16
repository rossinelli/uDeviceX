void clist_ini(int LX, int LY, int LZ, /**/ Clist **cl) {
    Clist *c;
    EMALLOC(1, cl);
    c = *cl;
    c->dims.x = LX;
    c->dims.y = LY;
    c->dims.z = LZ;
    c->ncells = LX * LY * LZ;

    int n = (c->ncells + 16);
    Dalloc(&c->starts, n);
    Dalloc(&c->counts, n);
}

void clist_ini_map(int maxp, int nA, const Clist *c, /**/ ClistMap **map) {
    long i, size;
    EMALLOC(1, map);
    ClistMap *m = *map;
    
    UC(scan_ini(c->ncells + 16, /**/ &m->scan));

    m->maxp = maxp;
    
    m->nA = nA;
    EMALLOC(nA, &m->ee);
    
    for (i = 0; i < nA; ++i) Dalloc(&m->ee[i], maxp);

    size = nA * maxp;
    Dalloc(&m->ii, size);
}
