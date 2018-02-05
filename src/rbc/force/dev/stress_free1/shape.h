struct Shape0 {
    real a; /* edge equilibrium lengths */
    real A; /* local area */
    real totArea;
};

/* extract edge specific shape info */
static void __device__ edg_shape(Shape shape, int i, /**/ Shape0 *shape0) {
    shape0->a = shape.a[i];
    shape0->A = shape.a[i];
    shape0->totArea = shape.totArea;
}
