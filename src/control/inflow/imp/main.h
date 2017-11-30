void ini(int2 nc, Inflow **i) {
    int n;
    size_t sz;
    Inflow *ip;
    Desc *d;
    
    UC(emalloc(sizeof(Inflow), (void**) i));
    
    ip = *i;
    d = &ip->d;

    n = nc.x * nc.y;
    d->nc = nc;
    
    sz = n * sizeof(curandState_t);
    CC(d::Malloc((void**) &d->rnds, sz));

    sz = n * sizeof(float3);
    CC(d::Malloc((void**) &d->uu, sz));

    sz = n * sizeof(float);
    CC(d::Malloc((void**) &d->cumflux, sz));

    sz = sizeof(int);
    CC(d::Malloc((void**) &d->ndev, sz));

    // TODO 
    plate::Params p;
    plate::VParams vp;

    p.o = make_float3(-XS/2, 0,     -ZS/2);
    p.a = make_float3(    0,  YS/2,     0);
    p.b = make_float3(    0,     0,  ZS);

    vp.u = make_float3(10.f, 0, 0);
    vp.upoiseuille = true;
    vp.upoiseuille = false;

    KL(plate::ini_vel, (k_cnf(nc.x * nc.y)), (vp, p, nc, /**/ d->uu));

    ip->p.plate = p;
    ip->vp.plate = vp;
}

void fin(Inflow *i) {
    Desc *d = &i->d;
    CC(d::Free(d->rnds));
    CC(d::Free(d->uu));
    CC(d::Free(d->cumflux));
    CC(d::Free(d->ndev));
    UC(efree(i));
}


void create_pp(Inflow *i, int *n, Particle *pp) {
    int2 nc;
    Desc *d;
    plate::Params p;
    
    d = &i->d;
    nc = d->nc;
    
    CC(d::MemcpyAsync(d->ndev, n, sizeof(int), H2D));
    
    p = i->p.plate;
    
    KL(plate::cumulative_flux, (k_cnf(nc.x * nc.y)), (p, nc, d->uu, /**/ d->cumflux));
    KL(plate::create_particles, (k_cnf(nc.x * nc.y)),
       (p, nc, d->uu, /*io*/ d->rnds, d->cumflux, /**/ d->ndev, pp));
    
    CC(d::MemcpyAsync(n, d->ndev, sizeof(int), D2H));
    dSync(); // wait for n
}
