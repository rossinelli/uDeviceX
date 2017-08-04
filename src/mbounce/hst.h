namespace mbounce {
namespace sub {
namespace hst {

void bounce(const Force *ff, const int *tt, int nt, int nv, const Particle *i_pp, const int *tcellstarts, const int *tcellcounts, const int *tids,
            const int n, /**/ Particle *pp, Momentum *mm) {

    for (int i = 0; i < n; ++i) {
        const Particle p1 = pp[i];

        Particle p0;
        rvprev(p1.r, p1.v, ff[i].f, /**/ p0.r, p0.v);

        const int xcid_ = int (p1.r[X] + XS/2);
        const int ycid_ = int (p1.r[Y] + YS/2);
        const int zcid_ = int (p1.r[Z] + ZS/2);

        float h = 2*dt; // must be higher than any valid result
        float rw[3], vw[3];

        int icol = -1; /* id of the collision triangle */

        for (int zcid = max(zcid_-1, 0); zcid <= min(zcid_ + 1, ZS - 1); ++zcid)
            for (int ycid = max(ycid_-1, 0); ycid <= min(ycid_ + 1, YS - 1); ++ycid)
                for (int xcid = max(xcid_-1, 0); xcid <= min(xcid_ + 1, XS - 1); ++xcid) {
                    const int cid = xcid + XS * (ycid + YS * zcid);
                    const int start = tcellstarts[cid];
                    const int count = tcellcounts[cid];

                    for (int j = start; j < start + count; ++j) {
                        const int tid = tids[j];
                        const int it  = tid % nt;
                        const int mid = tid / nt;

                        if (find_better_intersection(tt, it, i_pp + mid * nv, &p0, /*io*/ &h, /**/ rw, vw))
                            icol = tid;
                    }
                }

        if (icol != -1) {
            Particle pn;
            bounce_back(&p0, rw, vw, h, /**/ &pn);

            float dP[3], dL[3];
            lin_mom_change(    p1.v, pn.v, /**/ dP);
            ang_mom_change(rw, p1.v, pn.v, /**/ dL);

            pp[i] = pn;

            for (int c = 0; c < 3; ++c) {
                mm[icol].P[c] += dP[c];
                mm[icol].L[c] += dL[c];
            }
        }
    }
}

void collect_rig_mom(const Momentum *mm, int ns, int nt, /**/ Solid *ss) {
    int i = 0;
    Momentum m;
    
    for (int is = 0; is < ns; ++is) {
        for (int it = 0; it < nt; ++it, ++i) {
            
            m = mm[i];
    
            if (nonzero(&m)) {

                mom_shift_ref(ss[is].com, /**/ &m); 

                const float fac = dpd_mass / dt;

                for (int c = 0; c < 3; ++c) {
                    ss[is].fo[c] += fac * m.P[c];
                    ss[is].to[c] += fac * m.L[c];
                }
            }
        }
    }
}

void collect_rbc_mom(const Momentum *mm, int nc, int nt, int nv, const int4 *tt, const Particle *pp, /**/ Force *ff) {
    int i = 0, ic, it, off;
    int4 t;
    Particle pa, pb, pc;
    Momentum m;
    float fa[3], fb[3], fc[3];
    
    for (ic = 0; ic < nc; ++ic) {
        for (it = 0; it < nt; ++it, ++i) {
            
            m = mm[i];
    
            if (nonzero(&m)) {
                t = tt[it];
                off = ic * nv;
                
                pa = pp[off + t.x];
                pb = pp[off + t.y];
                pc = pp[off + t.z];

                M2f(m, pa.r, pb.r, pc.r, /**/ fa, fb, fc);

                for (int c = 0; c < 3; ++c) {
                    ff[off + t.x].f[c] += fa[c];
                    ff[off + t.y].f[c] += fb[c];
                    ff[off + t.z].f[c] += fc[c];
                }
            }
        }
    }
}

} // hst
} // sub
} // mbounce
