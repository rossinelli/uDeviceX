static __device__ bool valid_vel(float v, int L) {
    float dx = fabs(v * dt);
    if (dx > L/2)
        return false;
    return true;
}

static __device__ bool valid_vel3(float vx, float vy, float vz) {
    bool ok = true;
    ok &= valid_vel(vx, XS);
    ok &= valid_vel(vy, YS);
    ok &= valid_vel(vz, ZS);

    return ok;
}

static __device__ err_type valid_vv(const Particle p, bool verbose) {
    enum {X, Y, Z};
    err_type e;
    const float *v = p.v;
    e = check_float3(v);
    if (e != ERR_NONE) return e;
    if ( valid_vel3(v[X], v[Y], v[Z])) e = ERR_NONE;
    else                               e = ERR_INVALID;

    if (verbose && e != ERR_NONE)
        printf("DBG: vel: p = [%g, %g %g], [%g, %g, %g]\n",
               p.r[X], p.r[Y], p.r[Z], p.v[X], p.v[Y], p.v[Z]);

    return e;
}

__global__ void check_vv(const Particle *pp, int n, bool verbose = true) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    Particle p;
    if (i >= n) return;
    p = pp[i];
    err_type e = valid_vv(p, verbose);
    report(e);
}
