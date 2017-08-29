void update_solid0() {
    cD2H(s::q.pp_hst, s::q.pp, s::q.n);
    cD2H(s::ff_hst, s::ff, s::q.n);
  
    rig::update(s::ff_hst, s::q.rr0_hst, s::q.n, s::q.ns, /**/ s::q.pp_hst, s::q.ss_hst);
    rig::update_mesh(s::q.ss_hst, s::q.ns, s::q.m_hst, /**/ s::q.i_pp_hst);
  
    // for dump
    memcpy(s::q.ss_dmp, s::q.ss_hst, s::q.ns * sizeof(Solid));
  
    rig::reinit_ft(s::q.ns, /**/ s::q.ss_hst);
  
    cH2D(s::q.pp, s::q.pp_hst, s::q.n);
}
