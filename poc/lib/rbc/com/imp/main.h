static void reini(int nm, /**/ float3 *h) {
    if (nm) CC(d::MemsetAsync(h, 0, nm * sizeof(float3)));
}

static void reduce(int nm, int nv, const Particle *pp, /**/ float3 *rr, float3 *vv) {
    dim3 thrd(128, 1);
    dim3 blck(ceiln(nv, thrd.x), nm);

    KL(rbccom_dev::reduce_props, (blck, thrd), (nv, pp, /**/ rr, vv));
}

static void download(int nm, const float3 *d, /**/ float3 *h) {
    if (nm) CC(d::MemcpyAsync(h, d, nm * sizeof(float3), D2H));
}

static void normalize(int nm, int nv, /**/ float3 *h) {
    float fac = 1.f / nv;
    for (int i = 0; i < nm; ++i)
        scal(fac, h + i);
}

static void compute(int nm, const Particle *pp, /**/ RbcCom *q) {
    int nv;
    nv = q->nv;

    reini(nm, /**/ q->drr);
    reini(nm, /**/ q->dvv);

    reduce(nm, nv, pp, /**/ q->drr, q->dvv);

    download(nm, q->drr, /**/ q->hrr);
    download(nm, q->dvv, /**/ q->hvv);
    dSync();

    normalize(nm, nv, /**/ q->hrr);
    normalize(nm, nv, /**/ q->hvv);
}

void rbc_com_apply(RbcCom *q, int nm, const Particle *pp, /**/ float3 **prr, float3 **pvv) {
    int max_cell;
    max_cell = q->max_cell;
    if (nm > max_cell)
        ERR("nm=%d > max_cell=%d", nm, max_cell);
    if (!d::is_device_pointer(pp))
        ERR("`pp` is not a device pointer");
    compute(nm, pp, /**/ q);
    *prr = q->hrr; *pvv = q->hvv;
}
