struct WvelShear {
    float3 gdot; // shear rate in all three directions
};

struct WvelParam {
    union {
        WvelShear shear;
    } p;
    int type;
};

static __device__ void wvel_shear(WvelParam p, Coords c, float3 r, /**/ float3 *v) {
    float3 rc; // relative to center
    float3 gdot = p.p.shear.gdot;
    local2center(c, r, /**/ &rc);

    v->x = gdot.x * rc.x;
    v->y = gdot.y * rc.y;
    v->z = gdot.z * rc.z;
}

static __device__ void bounce_vel(WvelParam p, Coords c, float3 rw, /* io */ float3* v) {
    float3 vw;
    wvel(p, c, rw, /**/ &vw);
    v->x = 2 * vw.x - v->x;
    v->y = 2 * vw.y - v->y;
    v->z = 2 * vw.z - v->z;
}
