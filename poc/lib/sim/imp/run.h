_S_ void pre_run(Sim *s) {
    s->equilibrating = false;
    
    UC(dump_strt_templ(s));
    UC(utils_compute_hematocrit(s));
}

_S_ void step(TimeLine *time, float dt, float tstart, Sim *s) {
    long it;
    Flu *flu = &s->flu;
    Wall *wall = s->wall;
    BForce *bforce = s->bforce;
    const Opt *opt = &s->opt;

    UC(update_params(dt, s));
    
    if (active_walls(s))
        wall_update_vel(time_line_get_current(time) - tstart, wall);

    UC(check_sizes(s));
    UC(check_pos_soft(s));

    UC(distribute_flu(s));    
    UC(objects_distribute(s->obj));

    if (objects_have_bounce(s->obj)) {
        if (is_mbr_bb_activation_time(s))
            activate_mbr_bb(s);

        UC(store_solvent(&s->flu));
        UC(objects_save_mesh(s->obj));
    }

    UC(check_sizes(s));
    UC(forces(dt, time, bforce, s));
    UC(check_forces(dt, s));

    it = time_line_get_iteration(time);
    UC(field_sample(s));
    UC(dump(time, s));

    UC(restrain(it, /**/ s));
    UC(update_solvent(dt, /**/ flu));
    UC(objects_advance(dt, s->obj));
    UC(recolor_from_rbc(it, s));

    UC(check_vel(dt, s));
    if (opt->vcon && !s->equilibrating) {
        sample_vcont(s->coords, it, flu, /**/ &s->vcon);
        adjust_bforce(it, /**/ &s->vcon, bforce);
        log_vcont(it, &s->vcon);
    }

    if (active_walls(s)) bounce_wall(dt, s);

    UC(bounce_objects(dt, s));

    UC(check_pos_soft(s));
    UC(check_vel(dt, s));

    if (!s->equilibrating) {
        if (opt->inflow)     UC(apply_inflow(s->opt.params.kBT, s->opt.params.numdensity, dt, s->inflow, /**/ flu));
        if (opt->outflow)    UC(mark_outflow(flu, /**/ s->outflow));
        if (opt->denoutflow) UC(mark_outflowden(s->opt.params, flu, s->mapoutflow, /**/ s->denoutflow));
        if (opt->flu.colors) UC(recolor_flux(s->coords, &s->recolorer, flu));
        if (opt->flu.colors) UC(recolor_tracers(s->coords, &s->tracers, it, flu));
    }
}

/* ts, te: time start and end */
_I_ void run(float ts, float te, Sim *s) {
    float dt;

    dt = utils_get_dt0(s);
    while (time_line_get_current(s->time.t) < te) {
        UC(step(s->time.t, dt, ts, s));
        time_line_advance(dt, s->time.t);
        dt = utils_get_dt(s, s->time.t);
    }
    UC(distribute_flu(/**/ s));
}
