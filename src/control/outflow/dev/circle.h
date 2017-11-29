struct Params {
    float Rsq;  /* radius squared                */
    int inside; /* -1 for inside, +1 for outside */
};

__device__ int predicate(float3 o, Params p, const float r[3]) {
    enum {X, Y, Z};
    float x, y, z, rsq;
    x = r[X] - o.x;
    y = r[Y] - o.y;
    z = r[Z] - o.z;

    rsq = x*x + y*y + z*z;
    return (rsq - p.Rsq) * p.inside > 0;
}
