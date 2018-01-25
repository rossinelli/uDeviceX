__device__ real3 tri(RbcParams_v par, real3 r1, real3 r2, real3 r3, Shape0 shape, real area, real volume) {
    real l0, A0, totArea;
    int nv;
    totArea = RBCtotArea; nv = RBCnv;
    A0 = totArea / (2.0 * nv - 4.0);
    l0 = sqrt(A0 * 4.0 / sqrt(3.0));
    return tri0(par, r1, r2, r3,   l0, A0, totArea,   area, volume);
}

__device__ real3 dih(RbcParams_v par, real3 r0, real3 r1, real3 r2, real3 r3, real3 r4) {
    real3 f1, f2;
    f1 = dih0<1>(par, r0, r2, r1, r4);
    f2 = dih0<2>(par, r1, r0, r2, r3);
    add(&f1, /**/ &f2);
    return f2;
}
