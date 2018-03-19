#define PATTERN "%s/%s/%05d"
enum { NVP = 3 /* number of vertices per face */ };

static void copy_v(const Particle *f, /**/ Particle *t) {
    enum {X, Y, Z};
    t->v[X] = f->v[X];
    t->v[Y] = f->v[Y];
    t->v[Z] = f->v[Z];
}
static void shift(const Coords *c, const Particle *f, int n, /**/ Particle *t) {
    /* f, t: from, to
       see mesh/shift/     */
    int i;
    for (i = 0; i < n; i++) {
        shift0(c, f, t); copy_v(f, t); f++; t++;
    }
}

static void header(MPI_Comm cart, int nc0, int nv, int nt, WriteFile *f) {
    int nc; /* total number of cells */
    int sz = 0;
    char s[BUFSIZ] = {0};
    write_reduce(cart, nc0, &nc);
    if (m::is_master(cart))
        sz = sprintf(s,
                     "ply\n"
                     "format binary_little_endian 1.0\n"
                     "element vertex %d\n"
                     "property float x\nproperty float y\nproperty float z\n"
                     "property float u\nproperty float v\nproperty float w\n"
                     "element face  %d\n"
                     "property list int int vertex_index\n"
                     "end_header\n", nv*nc, nt*nc);
    write_master(cart, s, sz, f);
}

static void vert(MPI_Comm cart,const Particle *pp, int nc, int nv, WriteFile *f) {
    int n;
    n = nc * nv;
    UC(write_all(cart, pp, sizeof(Particle) * n, f));
}

static void wfaces0(MPI_Comm cart, int *buf, const int4 *faces, int nc, int nv, int nt, WriteFile *f) {
    /* write faces */
    int c, t, b;  /* cell, triangle, buffer index */
    int n, shift;
    n = nc * nv;
    write_shift_indices(cart, n, &shift);

    int4 tri;
    for(b = c = 0; c < nc; ++c)
        for(t = 0; t < nt; ++t) {
            tri = faces[t];
            buf[b++] = NVP;
            buf[b++] = shift + nv*c + tri.x;
            buf[b++] = shift + nv*c + tri.y;
            buf[b++] = shift + nv*c + tri.z;
        }
    UC(write_all(cart, buf, b*sizeof(buf[0]), f));
}

static void wfaces(MPI_Comm cart, const int4 *faces, int nc, int nv, int nt, WriteFile *f) {
    int *buf; /* buffer for faces */
    int sz;
    sz = (1 + NVP) * nc * nt;
    EMALLOC(sz, &buf);
    UC(wfaces0(cart, buf, faces, nc, nv, nt, f));
    EFREE(buf);
}

static void mesh_write0(MPI_Comm cart, const Particle *pp, const int4 *faces,
                   int nc, int nv, int nt, WriteFile *f) {
    UC(header(cart, nc,        nv, nt, f));
    UC(vert(cart, pp,      nc, nv,     f));
    UC(wfaces(cart, faces, nc, nv, nt, f));
}

static void mesh_write1(MPI_Comm cart, const Coords *c, const Particle *pp, const int4 *faces, int nc, int nv, int nt, WriteFile *f) {
    int n;
    Particle *pp0;
    n = nc * nv;
    EMALLOC(n, &pp0);
    shift(c, pp, n, /**/ pp0); /* copy-shift to global coordinates */
    UC(mesh_write0(cart, pp0, faces, nc, nv, nt, f));
    EFREE(pp0);
}

void mesh_write_particles(MeshWrite *q, MPI_Comm comm, const Coords *coords, int nc, const Particle *pp, int id) {
    int nv, nt, n;
    const int4* tt;
    Vectors *pos;
    Vectors *vel;
    WriteFile *f;
    const char *directory;
    char path[FILENAME_MAX];
    nv = q->nv; nt = q->nt; tt = q->tt; directory = q->directory;
    n = nv * nc;
    
    if (sprintf(path, PATTERN, DUMP_BASE, directory, id) < 0)
        ERR("sprintf failed");
    UC(write_file_open(comm, path, /**/ &f));

    vectors_postions_edge_ini(coords, n, pp, /**/ &pos);
    vectors_velocities_ini(n, pp, /**/ &vel);

    UC(mesh_write1(comm, coords, pp, tt, nc, nv, nt, f));

    vectors_fin(pos);
    vectors_fin(vel);
    UC(write_file_close(f));
}
