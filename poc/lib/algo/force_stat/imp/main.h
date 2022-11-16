static __device__ float sumsq(Force f) {
    enum {X, Y, Z};
    float x, y, z;
    x = f.f[X]; y = f.f[Y]; z = f.f[Z];
    return x*x + y*y + z*z;
}

struct SumSq {
    __device__ float operator()(Force f) const { return sumsq(f); }
};

float force_stat_max(int n, const Force *dev) {
    using namespace thrust;
    if (!d::is_device_pointer(dev))
        ERR("`dev` is not a device pointer");
    float init, m;
    SumSq           unary;
    maximum<float> binary;
    device_ptr<const Force> beg(dev), end(dev+n);
    init = 0;
    try {
        m = transform_reduce(beg, end, unary, init, binary);
    } catch (system_error e) {
        msg_print("thrust::transform_reduce failed");
        ERR("%s", e.what());
    }
    return sqrt(m);
}
