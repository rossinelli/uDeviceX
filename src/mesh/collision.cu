#include <stdio.h>
#include <assert.h>

#include <conf.h>
#include "inc/conf.h"
#include "inc/def.h"
#include "utils/error.h"
#include "utils/msg.h"
#include "utils/cc.h"

#include "d/q.h"
#include "d/ker.h"
#include "d/api.h"

#include "inc/type.h"
#include "inc/dev.h"
#include "utils/texo.h"
#include "utils/te.h"
#include "utils/texo.dev.h"

#include "utils/kl.h"
#include "mesh/collision.h"
#include "rigid/imp.h"

enum {OUT=BLUE_COLOR, IN=RED_COLOR};
enum {X, Y, Z};

static __host__ __device__ bool same_side(const float *x, const float *p, const float *a, const float *b, const float *inplane) {
    const float n[3] = {a[Y] * b[Z] - a[Z] * b[Y],
                        a[Z] * b[X] - a[X] * b[Z],
                        a[X] * b[Y] - a[Y] * b[X]};

    const float ndx = n[X] * (x[X] - inplane[X]) + n[Y] * (x[Y] - inplane[Y]) + n[Z] * (x[Z] - inplane[Z]);
    const float ndp = n[X] * (p[X] - inplane[X]) + n[Y] * (p[Y] - inplane[Y]) + n[Z] * (p[Z] - inplane[Z]);

    return ndx * ndp > 0;
}

static __host__ __device__ bool in_tetrahedron(const float *x, const float *A, const float *B, const float *C, const float *D) {
    const float AB[3] = {B[X] - A[X], B[Y] - A[Y], B[Z] - A[Z]};
    const float AC[3] = {C[X] - A[X], C[Y] - A[Y], C[Z] - A[Z]};
    const float AD[3] = {D[X] - A[X], D[Y] - A[Y], D[Z] - A[Z]};

    const float BC[3] = {C[X] - B[X], C[Y] - B[Y], C[Z] - B[Z]};
    const float BD[3] = {D[X] - B[X], D[Y] - B[Y], D[Z] - B[Z]};

    return
        same_side(x, A, BC, BD, D) &&
        same_side(x, B, AD, AC, D) &&
        same_side(x, C, AB, BD, D) &&
        same_side(x, D, AB, AC, A);
}

int collision_inside_1p(const RigPinInfo *pi, const float *r, const float *vv, const int4 *tt, const int nt) {
    int c = 0;
    float origin[3] = {0, 0, 0};
    int spdir = rig_get_pdir(pi);

    if (spdir != NOT_PERIODIC)
        origin[spdir] = r[spdir];

    for (int i = 0; i < nt; ++i) {
        int4 t = tt[i];
        if (in_tetrahedron(r, vv + 3*t.x, vv + 3*t.y, vv + 3*t.z, origin)) ++c;
    }
    return c%2;
}

namespace collisiondev
{
__global__ void init_tags(const int n, const int color, /**/ int *tags) {
    const int gid = threadIdx.x + blockIdx.x * blockDim.x;
    if (gid < n) tags[gid] = color;
}

union Pos {
    float2 f2[2];
    struct { float r[3]; float dummy; };
};

__device__ Pos tex2Pos(const Texo<float2> texvert, const int id) {
    Pos r;
    r.f2[0] = fetch(texvert, 3 * id + 0);
    r.f2[1] = fetch(texvert, 3 * id + 1);
    return r;
}

static __device__ bool inside_box(const float r[3], float3 lo, float3 hi) {
    enum {X, Y, Z};
    return
        r[X] >= lo.x && r[X] <= hi.x &&
        r[Y] >= lo.y && r[Y] <= hi.y &&
        r[Z] >= lo.z && r[Z] <= hi.z;
}

/* assume nm blocks along y */
/* if the ith particle is inside jth mesh, sets tag[i] to IN (see enum in collision.h) */
__global__ void compute_colors_tex(int spdir, const Particle *pp, const int n, const Texo<float2> texvert, const int nv,
                                   const int4 *tri,
                                   const int nt, const float3 *minext, const float3 *maxext, /**/ int *cc) {
    const int sid = blockIdx.y;
    const int gid = threadIdx.x + blockIdx.x * blockDim.x;
    if (gid >= n) return;

    int count = 0;

    const Particle p = pp[gid];

    float3 lo, hi;
    lo = minext[sid];
    hi = maxext[sid];
    if (!inside_box(p.r, lo, hi)) return;

    float origin[3] = {0, 0, 0};

    if (spdir != NOT_PERIODIC)
        origin[spdir] = p.r[spdir];

    int mbase = nv * sid;
    for (int i = 0; i < nt; ++i) {
        const int4 t = tri[i];

        const Pos a = tex2Pos(texvert, mbase + t.x);
        const Pos b = tex2Pos(texvert, mbase + t.y);
        const Pos c = tex2Pos(texvert, mbase + t.z);

        if (in_tetrahedron(p.r, a.r, b.r, c.r, origin)) ++count;
    }

    // dont consider the case of inside several solids
    if (count % 2) atomicExch(cc + gid, IN);
}
}


/*
   n:  number of particles
   nm: number of meshes
   nt: number of triangles per mesh
   nv: number of vertices per mesh
*/
static void get_colors0(int spdir, const Particle *pp, int n,
                        const Texo<float2> texvert, const int4 *tri,
                        int nt, int nv, int nm,
                        const float3 *minext, const float3 *maxext, /**/ int *cc) {
    if (nm == 0 || n == 0) return;

    KL(collisiondev::init_tags, (k_cnf(n)), (n, OUT, /**/ cc));

    enum {THR = 128};
    dim3 thrd(THR, 1);
    dim3 blck(ceiln(n, THR), nm);

    KL(collisiondev::compute_colors_tex, (blck, thrd), (spdir, pp, n, texvert, nv, tri, nt, minext, maxext, /**/ cc));
}

void collision_get_colors(int spdir, const Particle *pp, int n,
                          const Particle *i_pp, const int4 *tri,
                          int nt, int nv, int nm,
                          const float3 *minext, const float3 *maxext, /**/ int *cc) {
    Texo<float2> texvert;
    if (nm == 0 || n == 0) return;
    TE(&texvert, (float2*) i_pp, 3 * nm * nv);
    UC(get_colors0(spdir, pp, n, texvert, tri,
                   nt, nv, nm,
                   minext, maxext, /**/ cc));
    destroy(&texvert);
}

