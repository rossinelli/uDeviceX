void rbc_com_ini(int nv, int max_cell, /**/ RbcCom **pq) {
    RbcCom *q;
    size_t sz;
    EMALLOC(1, &q);

    sz = max_cell * sizeof(float3);
    CC(d::alloc_pinned((void**) &q->hrr, sz));
    CC(d::alloc_pinned((void**) &q->hvv, sz));
    Dalloc(&q->drr, max_cell);
    Dalloc(&q->dvv, max_cell);
    q->nv = nv; q->max_cell = max_cell;
    *pq = q;
}
