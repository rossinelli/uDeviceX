#include <mpi.h>
#include <stdio.h>

#include "utils/mc.h"
#include "utils/msg.h"
#include "mpi/glb.h"
#include "mpi/wrapper.h"
#include "utils/mc.h"
#include "utils/imp.h"
#include "utils/error.h"
#include "conf/imp.h"
#include "io/mesh_read/imp.h"

#include "mesh/eng_kantor/imp.h"
#include "mesh/scatter/imp.h"
#include "mesh/spherical/imp.h"

#include "io/vtk/imp.h"

#include "algo/vectors/imp.h"
#include "algo/scalars/imp.h"

struct Out {
    MPI_Comm comm;
    MeshRead *mesh;
    const char *path;
};

struct Spherical {
    double *r, *theta, *phi;
};

void spherical_ini(int n, Spherical **pq) {
    Spherical *q;
    EMALLOC(1, &q);
    EMALLOC(n, &q->r); EMALLOC(n, &q->theta); EMALLOC(n, &q->phi);
    *pq = q;
}

void spherical_fin(Spherical *q) {
    EFREE(q->r); EFREE(q->theta); EFREE(q->phi);
    EFREE(q);
}

static void dump_txt(int nv, int nm, Spherical *sph, const double *a) {
    int n, i;
    n = nv * nm;
    for (i = 0; i < n; i++)
        printf("%g %g %g %g\n", sph->r[i], sph->theta[i], sph->phi[i], a[i]);
}

static void dump_vtk(int nv, int nm, const double *eng, Vectors *vectors, Out *out) {
    int id;
    VTKConf *conf;
    VTK *vtk;
    MeshRead *mesh;
    const char *path;
    MPI_Comm comm;
    Scalars *scalars;

    mesh = out->mesh; comm = out->comm; path = out->path;
    scalars_double_ini(nv, eng, /**/ &scalars);

    vtk_conf_ini(mesh, &conf);
    vtk_conf_vert(conf, "eng");

    vtk_ini(comm, nm, path, conf, &vtk);
    vtk_points(vtk, nm, vectors);
    vtk_vert(vtk, nm, scalars, "eng");
    id = 0;
    vtk_write(vtk, comm, id);

    scalars_fin(scalars);
    vtk_fin(vtk);
    vtk_conf_fin(conf);
}

static void scatter(int nm, MeshRead *mesh, const double *edg, /**/ double *vert) {
    int ne;
    Scalars *scalars;
    MeshScatter *scatter;
    ne = mesh_read_get_ne(mesh);
    mesh_scatter_ini(mesh, /**/ &scatter);
    scalars_double_ini(nm * ne, edg, /**/ &scalars);
    UC(mesh_scatter_edg2vert_avg(scatter, nm, scalars, /**/ vert));
    scalars_fin(scalars);
    mesh_scatter_fin(scatter);
}

static void main0(const char *cell, Out *out) {
    int nv, ne, nm;
    MeshEngKantor *eng_kantor;
    const float *vert;
    Vectors  *pos;
    MeshSpherical *spherical;
    Spherical *sph;
    double *eng_edg, *eng_vert, kb, angle;
    nm = 1; kb = 1; angle = 0;
    UC(mesh_read_ini_off(cell, /**/ &out->mesh));
    UC(mesh_eng_kantor_ini(out->mesh, nm, /**/ &eng_kantor));
    nv = mesh_read_get_nv(out->mesh);
    spherical_ini(nv, &sph);
    UC(mesh_spherical_ini(nv, /**/ &spherical));
    ne = mesh_read_get_ne(out->mesh);

    vert = mesh_read_get_vert(out->mesh);
    UC(vectors_float_ini(nv, vert, /**/ &pos));

    EMALLOC(ne, &eng_edg);
    EMALLOC(nv, &eng_vert);

    mesh_eng_kantor_apply(eng_kantor, nm, pos,
                          kb, angle, /**/ eng_edg);
    mesh_spherical_apply(spherical, nm, pos, /**/ sph->r, sph->theta, sph->phi);
    scatter(nm, out->mesh, eng_edg, /**/ eng_vert);

    dump_vtk(nv, nm, eng_vert, pos, out);
    dump_txt(nv, nm, sph, eng_vert);

    mesh_eng_kantor_fin(eng_kantor);
    UC(vectors_fin(pos));
    UC(mesh_spherical_fin(spherical));
    UC(mesh_read_fin(out->mesh));
    EFREE(eng_vert);
    EFREE(eng_edg);
}

int main(int argc, char **argv) {
    const char *i;
    Out out;
    Config *cfg;
    int rank, size, dims[3];
    m::ini(&argc, &argv);
    m::get_dims(&argc, &argv, dims);
    m::get_cart(MPI_COMM_WORLD, dims, &out.comm);

    MC(m::Comm_rank(out.comm, &rank));
    MC(m::Comm_size(out.comm, &size));

    UC(conf_ini(&cfg));
    UC(conf_read(argc, argv, cfg));
    UC(conf_lookup_string(cfg, "i", &i));
    UC(conf_lookup_string(cfg, "o", &out.path));

    main0(i, &out);

    UC(conf_fin(cfg));
    MC(m::Barrier(out.comm));
    m::fin();
}