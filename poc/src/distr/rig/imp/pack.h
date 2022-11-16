static void pack_pp(const DMap m, int ns, int nv, const Particle *ipp, /**/ dBags *bags) {
    Sarray<Particle*, NBAGS> wrap;
    bag2Sarray(*bags, &wrap);

    dcommon_pack_pp_packets(ns, nv, ipp, m, /**/ wrap);
}

static void pack_ss(const DMap m, int n, const Rigid *ss, /**/ dBags *bags) {

    Sarray<Rigid*, 27> wrap;
    bag2Sarray(*bags, &wrap);

    KL((drig_dev::pack_ss), (k_cnf(n)), (ss, m, /**/ wrap));
}


void drig_pack(int ns, int nv, const Rigid *ss, const Particle *ipp, /**/ DRigPack *p) {
    UC(pack_pp(p->map, ns, nv, ipp, /**/ p->dipp));
    UC(pack_ss(p->map, ns, ss, /**/ p->dss));
}

void drig_download(DRigPack *p) {
    CC(d::Memcpy(p->hipp->counts, p->map.counts, NBAGS * sizeof(int), D2H));
    CC(d::Memcpy(p->hss->counts, p->map.counts, NBAGS * sizeof(int), D2H));
}
