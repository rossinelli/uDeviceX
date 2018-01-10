static void check_size(long n, long nmax) {
    if (n < 0 || n > nmax)
        ERR("wrong size: %ld / %ld", n, nmax);
}

static void check_sizes(Sim *s) {
    UC(check_size(s->rbc.q.nc, MAX_CELL_NUM));
    UC(check_size(s->rbc.q.n , MAX_CELL_NUM * RBCnv));
    UC(check_size(s->flu.q.n , MAX_PART_NUM)); 
}

void step(BForce *bforce, bool wall0, int it, Sim *s) {
    Flu *flu = &s->flu;
    Rbc *rbc = &s->rbc;
    Rig *rig = &s->rig;
    Wall *wall = &s->wall;
    
    UC(check_sizes(s));
    
    UC(distribute_flu(s));
    if (s->solids0) UC(distribute_rig(/**/ rig));
    if (rbcs)       UC(distribute_rbc(/**/ rbc));

    UC(check_sizes(s));
    
    forces(wall0, s);

    dump_diag0(s->coords, it, s);
    dump_diag_after(it, wall0, s->solids0, s);
    body_force(it, bforce, s);

    restrain(it, /**/ s);
    update_solvent(it, /**/ flu);
    if (s->solids0) update_solid(/**/ rig);
    if (rbcs)       update_rbc(it, rbc, s);

    if (s->opt.vcon && !s->equilibrating) {
        sample(s->coords, it, flu, /**/ &s->vcon);
        adjust(it, /**/ &s->vcon, bforce);
        log(it, &s->vcon);
    }

    if (wall0) bounce_wall(s->coords, wall, /**/ flu, rbc);

    if (sbounce_back && s->solids0) bounce_solid(it, /**/ &s->bb, rig, flu);

    if (! s->equilibrating) {
        if (s->opt.inflow)     apply_inflow(s->inflow, /**/ flu);
        if (s->opt.outflow)    mark_outflow(flu, /**/ s->outflow);
        if (s->opt.denoutflow) mark_outflowden(flu, s->mapoutflow, /**/ s->denoutflow);
    }
}
