void gen() { /* generate */
    run_eq(wall_creation);
    if (walls) {
        dSync();
        sdf::ini(&w::qsdf);
        MC(m::Barrier(m::cart));
        inter::create_walls(w::qsdf, /*io*/ &o::q, /**/ &w::q);
    }
    inter::freeze(w::qsdf, /*io*/ &o::q, /**/ &s::q, &r::q);
    clear_vel();

    if (multi_solvent) {
        Particle *pp = o::q.pp;
        int n = o::q.n;
        int *cc = o::qc.ii;
        Particle *pp_hst = a::pp_hst;
        int *cc_hst = o::qc.ii_hst;
        inter::color_dev(pp, n, /*o*/ cc, /*w*/ pp_hst, cc_hst);
    }
}

void sim_gen() {
    flu::gen_quants(&o::q, &o::qc);
    flu::build_cells(&o::q);
    if (global_ids)    flu::gen_ids  (o::q.n, &o::qi);
    flu::get_ticketZ(o::q, &o::tz);
    if (rbcs) {
        rbc::gen_quants("rbc.off", "rbcs-ic.txt", /**/ &r::q);
        rbc::gen_ticket(r::q, &r::tt);

        if (multi_solvent) gen_colors();
    }
    MC(m::Barrier(m::cart));

    long nsteps = (long)(tend / dt);
    MSG("will take %ld steps", nsteps);
    if (walls || solids) {
        solids0 = false;  /* global */
        gen();
        dSync();
        if (walls && w::q.n) wall::gen_ticket(w::q, &w::t);
        flu::get_ticketZ(o::q, &o::tz);
        solids0 = solids;
        if (rbcs && multi_solvent) gen_colors();
        run(wall_creation, nsteps);
    } else {
        solids0 = solids;
        run(            0, nsteps);
    }
    /* final strt dump*/
    if (strt_dumps) dump_strt(restart::FINAL);
}

void sim_strt() {
    long nsteps = (long)(tend / dt);

    /*Q*/
    flu::strt_quants(restart::BEGIN, &o::q);
    if (global_ids)    flu::strt_ii("id",     restart::BEGIN, &o::qi);
    if (multi_solvent) flu::strt_ii("colors", restart::BEGIN, &o::qc);
    flu::build_cells(&o::q);

    if (rbcs) rbc::strt_quants("rbc.off", restart::BEGIN, &r::q);
    dSync();

    if (solids) rig::strt_quants(restart::BEGIN, &s::q);

    if (walls) wall::strt_quants(&w::q);

    /*T*/
    flu::get_ticketZ(o::q, &o::tz);
    if (rbcs)            rbc::gen_ticket(r::q, &r::tt);
    if (walls && w::q.n) wall::gen_ticket(w::q, &w::t);

    MC(m::Barrier(m::cart));
    if (walls) {
        dSync();
        sdf::ini(&w::qsdf);
        MC(m::Barrier(m::cart));
    }

    solids0 = solids;

    MSG("will take %ld steps", nsteps - wall_creation);
    run(wall_creation, nsteps);

    if (strt_dumps) dump_strt(restart::FINAL);
}
