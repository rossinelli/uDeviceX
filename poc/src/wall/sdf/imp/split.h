/* sort solvent particle into remaining in solvent and turning into wall according to keys (all on hst) */
static void split_wall_solvent(const int *keys, /*io*/ int *s_n, Particle *s_pp, /**/ int *w_n, Particle *w_pp) {
    int n = *s_n;
    Particle p;
    int k, ia = 0, is = 0, iw = 0; /* all, solvent, wall */

    for (ia = 0; ia < n; ++ia) {
        k = keys[ia];
        p = s_pp[ia];
        
        if      (k == LABEL_BULK) s_pp[is++] = p;
        else if (k == LABEL_WALL) w_pp[iw++] = p;
    }
    *s_n = is;
    *w_n = iw;
}

/* sort solvent particle (dev) into remaining in solvent (dev) and turning into wall (hst)*/
void sdf_bulk_wall(const Sdf *sdf, /*io*/ int* s_n, Particle *s_pp, /*o*/ int *w_n, Particle *w_pp) {
    int n = *s_n, *labels;
    Particle *s_pp_hst;
    EMALLOC(n, &s_pp_hst);
    EMALLOC(n, &labels);
    
    UC(wall_label_hst(sdf, n, s_pp, labels));
    cD2H(s_pp_hst, s_pp, n);

    UC(split_wall_solvent(labels, /*io*/ s_n, s_pp_hst, /**/ w_n, w_pp));
    cH2D(s_pp, s_pp_hst, *s_n);
                       
    EFREE(s_pp_hst);
    EFREE(labels);
}

/* bulk predicate : is in bulk? */
static bool bulkp(int *keys, int i) { return keys[i] == LABEL_BULK; }

static int who_stays0(int *keys, int nc, int nv, /*o*/ int *stay) {
    int c, v;  /* cell and vertex */
    int s = 0; /* how many stays? */
    for (c = 0; c < nc; ++c) {
        v = 0;
        while (v  < nv && bulkp(keys, v + nv * c)) v++;
        if    (v == nv) stay[s++] = c;
    }
    return s;
}

int sdf_who_stays(const Sdf *sdf, int n, const Particle *pp, int nc, int nv, /**/ int *stay) {
    int *labels;
    EMALLOC(n, &labels);
    UC(wall_label_hst(sdf, n, pp, /**/ labels));
    nc = who_stays0(labels, nc, nv, /**/ stay);
    EFREE(labels);
    return nc;
}
