__global__ void ini_vel(VParams vparams, Params params, int2 nc, /**/ float3 *uu) {
    int i, xcid, ycid;
    float3 u;
    float2 xi;
    i = threadIdx.x + blockIdx.x * blockDim.x;

    if (i >= nc.x * nc.y) return;

    xcid = i % nc.x;
    ycid = i / nc.x;

    xi.x = (xcid + 0.5f) / nc.x;
    xi.y = (ycid + 0.5f) / nc.y;

    coords2vel(vparams, params, xi, /**/ &u);
    
    uu[i] = u;
}

__global__ void cumulative_flux(Params params, int2 nc, const float3 *uu, /**/ float *cumflux) {
    int i, xcid, ycid;
    float dn;
    float3 normal, u;
    i = threadIdx.x + blockIdx.x * blockDim.x;

    if (i >= nc.x * nc.y) return;

    xcid = i % nc.x;
    ycid = i / nc.x;

    normal = get_normal(params, nc, xcid, ycid);
    u      = uu[i];
    dn = dt * numberdensity * dot<float>(&normal, &u);

    cumflux[i] += dn;
}

static __device__ Particle create_particle(Params params, int2 nc, int xcid, int ycid, float3 u, curandState_t *rg) {
    float2 xi;
    float3 r;
    float sigma;
    xi.x = (xcid + curand_uniform(rg)) / nc.x;
    xi.y = (ycid + curand_uniform(rg)) / nc.y;

    coords2pos(params, xi, /**/ &r);

    sigma = sqrt(kBT);
    u.x += curand_normal(rg) * sigma;
    u.y += curand_normal(rg) * sigma;
    u.z += curand_normal(rg) * sigma;
    
    return Particle({r.x, r.y, r.z, u.x, u.y, u.z});
}

__global__ void create_particles(Params params, int2 nc, const float3 *flux, /* io */ curandState_t *rnds, float *cumflux, /**/ int *n, Particle *pp) {
    int i, xcid, ycid, j, nnew, strt;
    float c;
    float3 f;
    curandState_t rndstate; 
    i = threadIdx.x + blockIdx.x * blockDim.x;
    xcid = i % nc.x;
    ycid = i / nc.x;

    if (i >= nc.x * nc.y) return;

    c = cumflux[i];
    nnew = int(c);

    if (nnew == 0) return;

    c -= nnew;
    cumflux[i] = c;

    f        = flux[i];
    rndstate = rnds[i];
    
    strt = atomicAdd(n, nnew);
    
    for (j = strt; j < strt + nnew; ++j)
        pp[j] = create_particle(params, nc, xcid, ycid, f, /*io*/ &rndstate);

    rnds[i] = rndstate;
}
