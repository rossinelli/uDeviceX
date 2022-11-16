enum {X, Y, Z};
static __device__  int small(double s) {
    const double eps = 1e-12;
    if      (s >  eps) return 0;
    else if (s < -eps) return 0;
    else               return 1;
}
static __device__ void append(double x, int i, float *a) { atomicAdd(&a[i], x); }
static __device__ double tri_area(const double a[3], const double b[3], const double c[3]) { return tri_dev::kahan_area(a, b, c); }
static __device__ double tri_dih(const double a[3], const double b[3], const double c[3], const double d[3]) {
    double x, y;
    tri_dev::dihedral_xy(a, b, c, d, /**/ &x, &y);
    return -atan2(y, x); /* TODO: */
}
static __device__ void vec_scalar(const double a[3], double s, /**/ double b[3]) {
    b[X] = s*a[X]; b[Y] = s*a[Y]; b[Z] = s*a[Z];
}
static __device__ void vec_minus(const double a[3], const double b[3], /**/ double c[3]) {
    c[X] = a[X] - b[X];
    c[Y] = a[Y] - b[Y];
    c[Z] = a[Z] - b[Z];
}
static __device__ double vec_dot(const double a[3], const double b[3]) {
   return a[X]*b[X] + a[Y]*b[Y] + a[Z]*b[Z];
}
static __device__ void vec_cross(const double a[3], const double b[3], /**/ double c[3]) {
   c[X] = a[Y]*b[Z]-b[Y]*a[Z];
   c[Y] = b[X]*a[Z]-a[X]*b[Z];
   c[Z] = a[X]*b[Y]-b[X]*a[Y];
}
static __device__ double vec_abs(const double a[3]) { return sqrt(vec_dot(a, a)); }

static __device__ void vec_copy(const double a[3], double b[3]) {
    b[X] = a[X]; b[Y] = a[Y]; b[Z] = a[Z];
}

static __device__ double edg_abs(const double a[3], const double b[3]) {
    double u[3];
    vec_minus(b, a, u);
    return vec_abs(u);
}

static __device__ double tri_cot(const double a[3], const double b[3], const double c[3]) { /* at `b' */
    double x, y, u[3], v[3];
    y = 2 * tri_area(a, b, c);
    vec_minus(a, b, u);
    vec_minus(c, b, v);
    x = vec_dot(u, v);
    return x/y; /* TODO: */
}

static __device__ void vec_linear_combination(double al, const double a[3], double be, const double b[3],
                            /**/ double c[3]) {
    c[X] = al*a[X] + be*b[X];
    c[Y] = al*a[Y] + be*b[Y];
    c[Z] = al*a[Z] + be*b[Z];
}

static __device__ void vec_negative(const double a[3], /**/ double b[3]) {
    b[X] = -a[X]; b[Y] = -a[Y]; b[Z] = -a[Z];
}
static __device__ void vec_norm(const double a[3], /**/ double b[3]) {
    double s;
    s = vec_abs(a);
    if (!small(s)) vec_scalar(a, 1/s, /**/ b);
    else vec_copy(a, b);
}

static __device__ void dedg_abs(double a[3], double b[3], /**/ double da[3], double db[3]) {
    double u[3], e[3];
    vec_minus(b, a,   u);
    vec_norm(u,   e);
    vec_copy(e,     db);
    vec_negative(e, da);
}

static __device__ void vec_scalar_append(const double a[3], double s, int i,
                                         /**/ float *f) {
    append(s*a[X], 3*i,     f);
    append(s*a[Y], 3*i + 1, f);
    append(s*a[Z], 3*i + 2, f);
}

static __device__ void tri_normal(const double a[3], const double b[3], const double c[3], /**/ double e[3]) {
    double u[3], v[3], n[3];
    vec_minus(b, a,   u);
    vec_minus(c, a,   v);
    vec_cross(u, v,   n);
    vec_norm(n,   e);
}

static __device__ void ddih_angle(const double a[3], const double b[3], const double c[3], const double d[3], /**/
                                  double da[3], double db[3], double dc[3], double dd[3]) {
    double n[3], k[3];
    double e, An, Ak, cn, bn, bk, ck;

    tri_normal(a, b, c,   n);
    tri_normal(c, b, d,   k);

    e = edg_abs(c, b);

    An = tri_area(a, b, c);
    Ak = tri_area(c, b, d);

    vec_scalar(n, e/(2*An),   da);
    vec_scalar(k, e/(2*Ak),   dd);

    cn = tri_cot(b, c, a);
    bn = tri_cot(a, b, c);

    bk = tri_cot(c, b, d);
    ck = tri_cot(d, c, b);

    vec_linear_combination(-bn/e, n, -bk/e, k,    dc);
    vec_linear_combination(-cn/e, n, -ck/e, k,    db);
}

static __device__ void tri_edg(const double a[3], const double b[3], const double c[3],
                               /**/ double ab[3], double bc[3], double ca[3]) {
    vec_minus(b, a,   ab);
    vec_minus(c, b,   bc);
    vec_minus(a, c,   ca);
}

static __device__ void dtri_area(double a[3], double b[3], double c[3],  /**/ double da[3], double db[3], double dc[3]) {
    double n[3], n2[3], ab[3], bc[3], ca[3];
    tri_normal(a, b, c,   n);
    vec_scalar(n, 0.5,   n2);
    tri_edg(a, b, c,   ab, bc, ca);
    vec_cross(n2, bc,   da);
    vec_cross(n2, ca,   db);
    vec_cross(n2, ab,   dc);
}

static __device__ void get(const Particle *pp, int i, /**/ double r[3]) {
    r[X] = pp[i].r[X];
    r[Y] = pp[i].r[Y];
    r[Z] = pp[i].r[Z];
}
static __device__ void get2(const Particle *pp, int i, int j,
                            /**/ double a[3], double b[3]) {
    get(pp, i, /**/ a);
    get(pp, j, /**/ b);
}
static __device__ void get3(const Particle *pp, int i, int j, int k,
                            /**/ double a[3], double b[3], double c[3]) {
    get(pp, i, /**/ a);
    get(pp, j, /**/ b);
    get(pp, k, /**/ c);
}
static __device__ void get4(const Particle *pp, int i, int j, int k, int l,
                            /**/ double a[3], double b[3], double c[3], double d[3]) {
    get(pp, i, /**/ a);
    get(pp, j, /**/ b);
    get(pp, k, /**/ c);
    get(pp, l, /**/ d);
}

__global__ void sum(int nv, const float *from, /**/ float *to) {
    int i, c;
    float s;
    i  = threadIdx.x + blockIdx.x * blockDim.x;
    c  = blockIdx.y;
    if (i < nv) s = from[c*nv + i]; else s = 0;
    s = warpReduceSum(s);
    if ((threadIdx.x % warpSize) == 0) atomicAdd(&to[c], s);
}

static __device__ void compute_area0(const Particle *pp, int4 tri, /**/ float *area) {
    double area0;
    double a[3], b[3], c[3];
    int i, j, k;
    i = tri.x; j = tri.y; k = tri.z;
    get3(pp, i, j, k, /**/ a, b, c);
    area0 = tri_area(a, b, c)/3;

    append(area0, i, area);
    append(area0, j, area);
    append(area0, k, area);
}

__global__ void compute_area(int nv, int nt, int nc,
                             const Particle *pp, const int4 *tri,
                             /**/ float *area) {
    int i;
    int t, c; /* triangle, cell */
    i = threadIdx.x + blockDim.x * blockIdx.x;
    if (i >= nt*nc) return;

    c = i / nt;
    t = i % nt;

    pp   += nv*c;
    area += nv*c;
    tri  += t;

    compute_area0(pp, *tri, /**/ area);
}

static __device__ void compute_theta_len0(const Particle *pp, int4 dih,
                                          /**/ float *theta, float *lentheta) {
    int i, j, k, l;
    double a[3], b[3], c[3], d[3], u[3];
    double len0, theta0, lentheta0;

    i = dih.x; j = dih.y; k = dih.z; l = dih.w;
    get4(pp, i, j, k, l, /**/ a, b, c, d);

    *theta = theta0 = tri_dih(a, b, c, d);
    vec_minus(c, b, u);
    len0 = vec_abs(u);
    lentheta0 = len0*theta0;

    append(lentheta0, j,  lentheta);
    append(lentheta0, k,  lentheta);
}

__global__ void compute_theta_len(int nv, int ne, int nc,
                                  const Particle *pp, const int4 *dih,
                                  /**/ float *theta, float *lentheta) {
    int i;
    int e, c; /* edge, cell */
    i = threadIdx.x + blockDim.x * blockIdx.x;
    if (i >= ne*nc) return;

    c = i / ne;
    e = i % ne;

    pp       += nv*c;
    theta    += ne*c + e;
    lentheta += nv*c;
    dih      += e;

    compute_theta_len0(pp, *dih, /**/ theta, lentheta);
}

__global__ void compute_mean_curv(int nc, float H0, float kad,
                                  const float *lentheta, const float *area,
                                  /**/ float *mean) {
    int i;
    float pi;
    pi = 3.141592653589793;
    i = threadIdx.x + blockDim.x * blockIdx.x;
    if (i >= nc) return;
    mean[i] = (lentheta[i]/4 - H0 * area[i])*(4*kad*pi/area[i]);
}

static __device__ void force_edg0(float H0, float curva_mean_area_tot,
                                  const Particle *pp, const int4 dih,
                                  float theta, const float *lentheta, const float *area,
                                  /**/ float *f, float *fad) {
    int j, k;
    double b[3], c[3], db[3], dc[3];
    double coef;

    j = dih.y; k = dih.z;
    get2(pp, j, k, /**/ b, c);
    dedg_abs(b, c, db, dc);

    coef = - ( (lentheta[j]/area[j]/4 - H0) + (lentheta[k]/area[k]/4 - H0) ) * theta;
    vec_scalar_append(db, coef, j, f);
    vec_scalar_append(dc, coef, k, f);
    coef = -curva_mean_area_tot/4 * theta;
    vec_scalar_append(db, coef, j, fad);
    vec_scalar_append(dc, coef, k, fad);
}

__global__ void force_edg(int nv, int ne, int nc, float H0,
                          const Particle *pp, const int4 *dih,
                          const float *curva_mean_area_tot,
                          const float *theta, const float *lentheta, const float *area,
                          /**/ float *f, float *fad) {
    int i;
    int e, c; /* edge, cell */
    i = threadIdx.x + blockDim.x * blockIdx.x;
    if (i >= ne*nc) return;

    c = i / ne;
    e = i % ne;

    pp       += nv*c;
    dih      += e;

    curva_mean_area_tot += c;
    theta    += ne*c + e;
    lentheta += nv*c;
    area     += nv*c;

    f        += 3*nv*c;
    fad      += 3*nv*c;

    force_edg0(H0, *curva_mean_area_tot,
               pp, *dih,
               *theta, lentheta, area, /**/ f, fad);
}

static __device__ void force_lentheta0(float H0, float curva_mean_area_tot,
                                       const Particle *pp, const int4 dih,
                                       const float *lentheta, const float *area,
                                       /**/ float *f, float *fad) {
    int i, j, k, l;
    double len0, coef;
    double a[3], b[3], c[3], d[3], u[3];
    double da[3], db[3], dc[3], dd[3];

    i = dih.x; j = dih.y; k = dih.z; l = dih.w;
    get4(pp, i, j, k, l, /**/ a, b, c, d);
    ddih_angle(a, b, c, d, /**/ da, db, dc, dd);
    vec_minus(c, b, u);
    len0 = vec_abs(u);

    coef =  -(  (lentheta[j]/area[j]/4 - H0) + (lentheta[k]/area[k]/4 - H0) ) * len0 ;

    vec_scalar_append(da, coef, i, f);
    vec_scalar_append(db, coef, j, f);
    vec_scalar_append(dc, coef, k, f);
    vec_scalar_append(dd, coef, l, f);

    coef = -curva_mean_area_tot/2.0*len0;
    vec_scalar_append(da, coef, i, fad);
    vec_scalar_append(db, coef, j, fad);
    vec_scalar_append(dc, coef, k, fad);
    vec_scalar_append(dd, coef, l, fad);
}

__global__ void force_lentheta(int nv, int ne, int nc, float H0,
                               const Particle *pp, const int4 *dih,
                               const float *curva_mean_area_tot,
                               const float *lentheta, const float *area,
                               /**/ float *f, float *fad) {
    int i;
    int e, c; /* edge, cell */
    i = threadIdx.x + blockDim.x * blockIdx.x;
    if (i >= ne*nc) return;

    c = i / ne;
    e = i % ne;

    pp       += nv*c;
    dih      += e;

    curva_mean_area_tot += c;
    lentheta += nv*c;
    area     += nv*c;

    f        += 3*nv*c;
    fad      += 3*nv*c;

    force_lentheta0(H0, *curva_mean_area_tot,
                    pp, *dih,
                    lentheta, area, /**/ f, fad);
}

static __device__ void force_area0(float H0, const Particle *pp, const int4 tri,
                                   const float *lentheta, const float *area,
                                   /**/ float *f) {
    int i, j, k;
    double a[3], b[3], c[3];
    double da[3], db[3], dc[3];
    double coef1, coef2, coef;

    i = tri.x; j = tri.y; k = tri.z;
    get3(pp, i, j, k, /**/ a, b, c);

    dtri_area(a, b, c, da, db, dc);

    coef1 = 1.0/3.0;

    coef2 = lentheta[i]*lentheta[i]/8.0/area[i]/area[i] - 2.0*H0*H0;
    coef = coef1 * coef2;
    vec_scalar_append(da, coef, i, f);

    coef2 = lentheta[j]*lentheta[j]/8.0/area[j]/area[j] - 2.0*H0*H0;
    coef = coef1 * coef2;
    vec_scalar_append(db, coef, j, f);

    coef2 = lentheta[k]*lentheta[k]/8.0/area[k]/area[k] - 2.0*H0*H0;
    coef = coef1 * coef2;
    vec_scalar_append(dc, coef, k, f);
}

__global__ void force_area(int nv, int nt, int nc, float H0,
                           const Particle *pp, const int4 *tri,
                           const float *lentheta, const float *area,
                           /**/ float *f) {
    int i;
    int t, c; /* triangle, cell */
    i = threadIdx.x + blockDim.x * blockIdx.x;
    if (i >= nt*nc) return;

    c = i / nt;
    t = i % nt;

    pp       += nv*c;
    tri      += t;

    lentheta += nv*c;
    area     += nv*c;

    f        += 3*nv*c;

    force_area0(H0, pp, *tri,
                lentheta, area, /**/ f);
}

__global__ void accumulate(int n,
                           const float *ff0, const float *ff1,
                           /**/ Force *ff) {
    enum {X, Y, Z};
    int i;
    Force *f;
    const float *f0, *f1;

    i = threadIdx.x + blockDim.x * blockIdx.x;
    if (i >= n) return;

    f0 = &ff0[3*i];
    f1 = &ff1[3*i];
    f = &ff[i];

    f->f[X] += f0[X] + f1[X];
    f->f[Y] += f0[Y] + f1[Y];
    f->f[Z] += f0[Z] + f1[Z];
}
