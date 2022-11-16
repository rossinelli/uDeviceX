static void ini_rnd(int n, curandState_t *rr) {
    long seed = 1234567;
    KL(inflow_dev::ini_rnd, (k_cnf(n)), (seed, n, rr));
}

static void ini_flux(int n, curandState_t *rr, float *cumflux) {
    KL(inflow_dev::ini_flux, (k_cnf(n)), (n, rr, cumflux));
}

void inflow_ini(int2 nc, Inflow **i) {
    int n;
    Inflow *ip;
    Desc *d;

    EMALLOC(1, i);

    ip = *i;
    d = &ip->d;

    n = nc.x * nc.y;
    d->nc = nc;

    Dalloc(&d->rnds,    n);
    Dalloc(&d->uu,      n);
    Dalloc(&d->cumflux, n);
    Dalloc(&d->ndev,    1);

    UC(ini_rnd(n, d->rnds));
    UC(ini_flux(n, d->rnds, d->cumflux));

    ip->t = TYPE_NONE;
}

static void ini_velocity(Type t, int2 nc, const ParamsU *p, const VParamsU *vp, /**/ float3 *uu) {
    int n = nc.x * nc.y;
    switch(t) {
    case TYPE_PLATE:
        KL(inflow_dev::ini_vel, (k_cnf(n)), (vp->plate, p->plate, nc, /**/ uu));
        break;
    case TYPE_CIRCLE:
        KL(inflow_dev::ini_vel, (k_cnf(n)), (vp->circle, p->circle, nc, /**/ uu));
        break;
    case TYPE_NONE:
        break;
    default:
        ERR("No inflow type is set");
        break;
    };
}

void inflow_ini_velocity(Inflow *i) {
    ini_velocity(i->t, i->d.nc, &i->p, &i->vp, /**/ i->d.uu);
}

void inflow_fin(Inflow *i) {
    Desc *d = &i->d;
    Dfree(d->rnds);
    Dfree(d->uu);
    Dfree(d->cumflux);
    Dfree(d->ndev);
    EFREE(i);
}

static void create_solvent(float kBT, int numdensity, float dt, Inflow *i, int *n, SolventWrap wrap) {
    int2 nc;
    Desc *d;
    int nctot;
    float nxdt;

    d = &i->d;
    nc = d->nc;
    nctot = nc.x * nc.y;
    nxdt = numdensity * dt;
    
    CC(d::MemcpyAsync(d->ndev, n, sizeof(int), H2D));

    switch(i->t) {
    case TYPE_PLATE:
        KL(inflow_dev::cumulative_flux, (k_cnf(nctot)), (nxdt, i->p.plate, nc, d->uu, /**/ d->cumflux));
        KL(inflow_dev::create_particles, (k_cnf(nctot)),
           (kBT, i->p.plate, nc, d->uu, /*io*/ d->rnds, d->cumflux, /**/ d->ndev, wrap));
        break;
    case TYPE_CIRCLE:
        KL(inflow_dev::cumulative_flux, (k_cnf(nctot)), (nxdt, i->p.circle, nc, d->uu, /**/ d->cumflux));
        KL(inflow_dev::create_particles, (k_cnf(nctot)),
           (kBT, i->p.circle, nc, d->uu, /*io*/ d->rnds, d->cumflux, /**/ d->ndev, wrap));
        break;
    case TYPE_NONE:
        break;
    default:
        ERR("No inflow type is set");
        break;
    };

    CC(d::MemcpyAsync(n, d->ndev, sizeof(int), D2H));
    dSync(); // wait for n
}

void inflow_create_pp(float kBT, int numdensity, float dt, Inflow *i, int *n, Particle *pp) {
    SolventWrap wrap;

    wrap.pp = pp;
    wrap.cc = NULL;
    wrap.multisolvent = false;
    wrap.color = -1;

    create_solvent(kBT, numdensity, dt, i, n, wrap);
}

void inflow_create_pp_cc(float kBT, int numdensity, float dt, int newcolor, Inflow *i, int *n, Particle *pp, int *cc) {
    SolventWrap wrap;

    wrap.pp = pp;
    wrap.cc = cc;
    wrap.multisolvent = true;
    wrap.color = newcolor;
    create_solvent(kBT, numdensity, dt, i, n, wrap);
}
