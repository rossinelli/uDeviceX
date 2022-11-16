enum {NTHRD=128};

void rig_pininfo_ini(RigPinInfo **pi) {
    RigPinInfo *p;
    EMALLOC(1, pi);
    p = *pi;
    p->pdir = NOT_PERIODIC;
}

void rig_pininfo_fin(RigPinInfo *pi) {
    EFREE(pi);
}

void rig_pininfo_set(int3 com, int3 axis, RigPinInfo *pi) {
    pi->com  = com;
    pi->axis = axis;
}

void rig_pininfo_set_pdir(int pdir, RigPinInfo *pi) {
    pi->pdir = pdir;
}

int rig_pininfo_get_pdir(const RigPinInfo *pi) {
    return pi->pdir;
}

void rig_reinit_ft(const int nsolid, /**/ Rigid *ss) {
    KL(rigid_dev::reinit_ft, (k_cnf(nsolid)), (nsolid, /**/ ss));
}

static bool pinned_com(const RigPinInfo *pi) {
    int3 com = pi->com;
    return com.x && com.y && com.z;
}

void rig_update(const RigPinInfo *pi, float dt, int n, const Force *ff, const float *rr0, int ns, /**/ Particle *pp, Rigid *ss) {
    if (ns < 1) return;

    const int nps = n / ns; /* number of particles per solid */

    const dim3 nblck ( ceiln(nps, NTHRD), ns );
    const dim3 nthrd ( NTHRD, 1 );

    KL(rigid_dev::add_f_to, ( nblck, nthrd ), (nps, pp, ff, /**/ ss));
    KL(rigid_dev::update_om_v, (1, ns), (*pi, dt, ns, /**/ ss));
    if (!pinned_com(pi))
        KL(rigid_dev::update_com, (1, 3*ns ), (dt, ns, /**/ ss));
    KL(rigid_dev::rot_referential, (1, ns), (dt, ns, /**/ ss));

    KL(rigid_dev::update_pp, ( nblck, nthrd ), (nps, rr0, ss, /**/ pp));
}

void rig_generate(int ns, const Rigid *ss, int nps, const float *rr0, /**/ Particle *pp) {
    if (ns < 1) return;

    const dim3 nblck ( ceiln(nps, NTHRD), ns );
    const dim3 nthrd ( NTHRD, 1 );

    KL(rigid_dev::update_pp, ( nblck, nthrd ), (nps, rr0, ss, /**/ pp));
}

void rig_update_mesh(float dt, int ns, const Rigid *ss, int nv, const float *vv, /**/ Particle *pp) {
    const dim3 nblck ( ceiln(nv, NTHRD), ns );
    const dim3 nthrd ( NTHRD, 1 );
    KL(rigid_dev::update_mesh, (nblck, nthrd ), (dt, ss, nv, vv, /**/ pp));
}
