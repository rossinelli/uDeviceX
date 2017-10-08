static __device__ void gen(Pa A, Pa B, float rnd, /**/ Fo *f) {
    enum {O = SOLVENT_KIND, S = SOLID_KIND, W = WALL_KIND};
    int ka, kb;
    float m[3];
    m[0] = 1; m[S] = rbc_mass; m[W] = 1;

    ka = A.kind; kb = B.kind;
    gen2(A, B, rnd, /**/ f);
}
