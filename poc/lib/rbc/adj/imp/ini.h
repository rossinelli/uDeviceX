static void gen_a12(int md, int i0, int *hx, int *hy, /**/ int *a1, int *a2) {
    int lo = i0*md, hi = lo + md, mi = hx[lo];
    int i;
    for (i = lo + 1; (i < hi) && (hx[i] != -1); i++)
        if (hx[i] < mi) mi = hx[i]; /* minimum */

    int c = mi, c0;
    i = lo;
    do {
        c     = edg_get(md, i0, c0 = c, hx, hy);
        a1[i] = c0;
        a2[i] = edg_get(md, c, c0, hx, hy);
        i++;
    }  while (c != mi);
}

static void ini0(int md, int nt, int nv, const int4 *faces, /**/ int *a1, int *a2) {
    int hx[nv*md], hy[nv*md];
    int i;
    for (i = 0; i < nv*md; i++) a1[i] = a2[i] = -1;
    edg_ini(nv, md, hx);

    int4 t;
    for (int ifa = 0; ifa < nt; ifa++) {
        t = faces[ifa];
        int f0 = t.x, f1 = t.y, f2 = t.z;
        edg_set(md, f0, f1, f2,   hx, hy); /* register an edge */
        edg_set(md, f1, f2, f0,   hx, hy);
        edg_set(md, f2, f0, f1,   hx, hy);
    }
    for (i = 0; i < nv; i++) gen_a12(md, i, hx, hy, /**/ a1, a2);
}

void adj_ini(int md, int nt, int nv, const int4 *faces, /**/ Adj **pq) {
    int *a1, *a2;
    Adj *q;
    EMALLOC(1    , &q);
    EMALLOC(md*nv, &q->adj0);
    EMALLOC(md*nv, &q->adj1);
    a1 = q->adj0; /* sic */
    a2 = q->adj1;
    q->md = md; q->nv = nv;
    ini0(md, nt, nv, faces, /**/ a1, a2);
    *pq = q;
}

void adj_view_ini(const Adj *hst, /**/ Adj_v **pq) {
    int nv, md;
    Adj_v *q;
    if (hst == NULL) ERR("hst == NULL");
    nv = hst->nv; md = hst->md;
    EMALLOC(1, &q);
    Dalloc(&q->adj0, md*nv);
    Dalloc(&q->adj1, md*nv);

    q->nv = nv; q->md = md;
    cH2D(q->adj0, hst->adj0, md*nv);
    cH2D(q->adj1, hst->adj1, md*nv);

    *pq = q;
}
