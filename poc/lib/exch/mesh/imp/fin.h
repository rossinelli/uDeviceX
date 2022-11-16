void emesh_pack_fin(EMeshPack *p) {
    UC(emap_fin(NFRAGS, /**/ &p->map));
    UC(comm_bags_fin(PINNED, NONE, /**/ p->hpp, p->dpp));
    UC(comm_buffer_fin(p->hbuf));
    Dfree(p->minext);
    Dfree(p->maxext);
    EFREE(p);
}

void emesh_comm_fin(EMeshComm *c) {
    UC(comm_fin(c->pp));
    EFREE(c);
}

void emesh_unpack_fin(EMeshUnpack *u) {
    UC(comm_bags_fin(PINNED_DEV, NONE, /**/ u->hpp, u->dpp));
    UC(comm_buffer_fin(u->hbuf));
    EFREE(u);
}

/* Momentum struct */

static void fin_map(/**/ MMap *map) {
    Dfree(map->cc);
    Dfree(map->ss);
    Dfree(map->subids);
}

void emesh_packm_fin(EMeshPackM *p) {
    UC(comm_bags_fin(PINNED, NONE, /**/ p->hmm, p->dmm));
    UC(comm_bags_fin(PINNED, NONE, /**/ p->hii, p->dii));

    for (int i = 0; i < NFRAGS; ++i)
        UC(fin_map(&p->maps[i]));

    CC(d::FreeHost(p->cchst));
    UC(comm_buffer_fin(p->hbuf));
    EFREE(p);
}

void emesh_commm_fin(EMeshCommM *c) {
    UC(comm_fin(c->comm));
    EFREE(c);
}

void emesh_unpackm_fin(EMeshUnpackM *u) {
    UC(comm_bags_fin(PINNED_DEV, NONE, /**/ u->hmm, u->dmm));
    UC(comm_bags_fin(PINNED_DEV, NONE, /**/ u->hii, u->dii));
    UC(comm_buffer_fin(u->hbuf));
    EFREE(u);
}
