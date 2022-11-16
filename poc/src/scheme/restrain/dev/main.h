static __device__ int valid_p(const MapGrey, int) { return 1; }
static __device__ int valid_p(const MapColor m, int i) {
    return m.cc[i] == m.color;
}

template <typename Map>
__global__ void sum(Map m, int n, const Particle *pp, /**/ int *ntot, float3 *vtot) {
    enum {X, Y, Z};
    int i, good, ngood;
    float3 v;

    i = threadIdx.x + blockDim.x * blockIdx.x;
    good = (i < n) && valid_p(m, i);

    if (good) {
        const Particle p = pp[i]; 
        v.x = p.v[X];
        v.y = p.v[Y];
        v.z = p.v[Z];
    } else {
        v.x = v.y = v.z = 0;
    }

    v     = warpReduceSum(v);
    ngood = warpReduceSum(good);

    if ((threadIdx.x % warpSize == 0) && ngood > 0) {
        atomicAdd(&vtot->x, v.x);
        atomicAdd(&vtot->y, v.y);
        atomicAdd(&vtot->z, v.z);
        atomicAdd(ntot, ngood);
    }
}

template <typename Map>
__global__ void shift(Map m, float3 v, int n, /**/ Particle *pp) {
    enum {X, Y, Z};
    int i, good;
    i = threadIdx.x + blockDim.x * blockIdx.x;

    good = (i < n) && valid_p(m, i);

    if (good) {
        Particle p = pp[i];
        p.v[X] -= v.x;
        p.v[Y] -= v.y;
        p.v[Z] -= v.z;
        pp[i] = p;
    }
}
