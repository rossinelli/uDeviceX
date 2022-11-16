void mesh_eng_julicher_ini(MeshRead *mesh, int nm, /**/ MeshEngJulicher **pq) {
    int nv, ne;
    MeshEngJulicher *q;
    EMALLOC(1, &q);

    UC(nv = mesh_read_get_nv(mesh));
    UC(ne = mesh_read_get_ne(mesh));

    q->max_nm = nm;
    q->nv = nv; q->ne = ne;
    EMALLOC(nm*ne, &q->lens);
    EMALLOC(nm*ne, &q->angles);
    EMALLOC(nm*nv, &q->areas);
    EMALLOC(nm*ne, &q->curvs_edg);
    EMALLOC(nm*nv, &q->curvs_vert);

    mesh_edg_len_ini(mesh, /**/ &q->len);
    mesh_angle_ini(mesh, /**/ &q->angle);
    mesh_vert_area_ini(mesh, /**/ &q->area);
    mesh_scatter_ini(mesh, /**/ &q->scatter);

    *pq = q;
}

void mesh_eng_julicher_fin(MeshEngJulicher *q) {
    EFREE(q->lens);
    EFREE(q->angles);
    EFREE(q->areas);
    EFREE(q->curvs_edg);
    EFREE(q->curvs_vert);    

    mesh_edg_len_fin(q->len);
    mesh_angle_fin(q->angle);
    mesh_vert_area_fin(q->area);
    mesh_scatter_fin(q->scatter);

    EFREE(q);
}

void mesh_eng_julicher_apply(MeshEngJulicher *q, int nm, Vectors *pos, double kb, /**/ double *o) {
    int i, nv, ne;
    double *lens, *angles, *areas, *curvs_vert, *curvs_edg;
    double area, curv;
    Scalars *sc;
    lens = q->lens; angles = q->angles; areas = q->areas;
    curvs_edg = q->curvs_edg; curvs_vert = q->curvs_vert;

    ne = q->ne; nv = q->nv;
    mesh_edg_len_apply(q->len, nm, pos, /**/ lens);
    mesh_angle_apply(q->angle, nm, pos, /**/ angles);
    for (i = 0; i < nm * ne; i++)
        curvs_edg[i] = lens[i]*angles[i];

    scalars_double_ini(nm * ne, curvs_edg, /**/ &sc);
    mesh_scatter_edg2vert(q->scatter, nm, sc, /**/ curvs_vert);
    for (i = 0; i < nm * nv; i++)
        curvs_vert[i] /= 4;

    mesh_vert_area_apply(q->area, nm, pos, /**/ areas);
    for (i = 0; i < nm * nv; i++) {
        curv = curvs_vert[i];
        area = areas[i];
        if (area <= 0) ERR("area[%d]=%g <= 0", area, i);
        o[i] = 2*kb*curv*curv/area;
    }
    scalars_fin(sc);
    mesh_vert_area_apply(q->area, nm, pos, /**/ areas);
}
