_I_ bool active_walls(const Sim *s) {
    return !s->equilibrating && s->opt.wall.active;
}

_I_ bool is_sampling_time(const Sim *s) {
    const Opt *opt = &s->opt;
    const float freq = opt->dump.freq_field / opt->sampler_npdump;
    const TimeLine *time = s->time.t;

    return time_line_cross(time, freq);
}

_I_ bool is_mbr_bb_activation_time(Sim *s) {
    float t, tbb;
    t = time_line_get_current(s->time.t);
    tbb = s->time.mbr_bb;

    if (t > tbb && !s->active_mbr_bb) {
        s->active_mbr_bb = true;
        return true;
    }
    return false;
}

_I_ void utils_compute_hematocrit(const Sim *s) {
    const double SMALL = 1e-3;
    const Opt *opt = &s->opt;
    double Vdomain, Vrbc, Ht;
    if (opt->wall.active) {
        Vdomain = wall_compute_volume(s->wall, s->cart, s->opt.params.L);
    }
    else {
        const Coords *c = s->coords;
        Vdomain = xdomain(c) * ydomain(c) * zdomain(c);
    }
    if (Vdomain < SMALL)
        ERR("Vdomain = %g", Vdomain);
    
    Vrbc = objects_mbr_tot_volume(s->obj);
    Ht = Vrbc / Vdomain;

    msg_print("Geometry volume: %g", Vdomain);
    msg_print("Hematocrit: %g", Ht);
}

_I_ void utils_dump_history(const Config *cfg, const char *fname) {
    FILE *f;
    UC(efopen(fname, "w", &f));
    UC(conf_write_history(cfg, f));
    UC(efclose(f));
}

_I_ float utils_get_dt0(Sim *s) {
    TimeStep *time_step;
    time_step = s->time.step;
    return time_step_dt0(time_step);
}

_S_ void push_accel(const Sim *s, TimeStepAccel *aa) {
    const Flu *flu = &s->flu;
    if (flu->q.n) UC(time_step_accel_push(aa, flu->mass, flu->q.n, flu->ff));
    UC(objects_get_accel(s->obj, aa));
}

_I_ float utils_get_dt(Sim *s, TimeLine *time) {
    /* Possibility to adapt dt only after equilibration */
    float dt;
    const Opt *opt = &s->opt;
    if (s->equilibrating)
        dt = time_step_dt0(s->time.step);
    else {
        UC(time_step_accel_reset(s->time.accel));
        UC(push_accel(s, /**/ s->time.accel));
        dt = time_step_dt(s->time.step, s->cart, s->time.accel);

        // TODO: move to dump_diag?
        if (time_line_cross(time, opt->dump.freq_diag))
            time_step_log(s->time.step);
    }
    return dt;
}

_I_ void utils_get_pf_flu(Sim *s, PFarray *flu) {
    Flu *f = &s->flu;
    flu->n = f->q.n;
    UC(parray_push_pp(f->q.pp, &flu->p));
    if (s->opt.flu.colors)
        parray_push_cc(f->q.cc, &flu->p);

    UC(farray_push_ff(f->ff, &flu->f));
}

_I_ void utils_set_n_flu_pf(const PFarray *flu, Sim *s) {
    Flu *f = &s->flu;
    f->q.n = flu->n;
}
