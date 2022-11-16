void dbg_ini(Dbg **dbg) {
    Dbg *d;
    int i;
    EMALLOC(1, dbg);
    d = *dbg;
    for (i = 0; i < DBG_NKIND_; ++i) d->state[i] = 0;
    d->verbose = false;
    d->dump = false;
}
void dbg_fin(Dbg *dbg) {
    EFREE(dbg);
}

static void set(int kind, int val, Dbg *dbg) {
    if (kind < 0 || kind >= DBG_NKIND_)
        ERR("Unrecognised kind of id <%d>", kind);
    dbg->state[kind] = val;
}

void dbg_enable(int kind, Dbg *dbg) {
    UC(set(kind, 1, dbg));
}

void dbg_disable(int kind, Dbg *dbg) {
    UC(set(kind, 0, dbg));
}

void dbg_set_verbose(bool active, Dbg *dbg) {dbg->verbose = active;}
void dbg_set_dump(bool active, Dbg *dbg) {dbg->dump = active;}

static int check(const Dbg *dbg, int kind) {return dbg->state[kind];}

static bool error() {
    err_type e;
    UC(e = err_get());
    return e != ERR_NONE;
} 

static void gen_name(const Coords *c, const char *base, /**/ char *name) {
    int r;
    char stamp[FILENAME_MAX];
    coord_stamp(c, /**/ stamp);
    r = sprintf(name, "dbg.%s.%s.txt", base, stamp);
    if (r < 0) ERR("sprintf failed");
}

static void dump_pp(const Coords *c, const char *base, int n, const Particle *dev) {
    Particle *hst;
    char name[FILENAME_MAX];
    EMALLOC(n, &hst);
    cD2H(hst, dev, n);
    UC(gen_name(c, base, name));
    UC(txt_write_pp(n, hst, name));
    EFREE(hst);    
}

static void dump_pp_ff(const Coords *c, const char *base, int n, const Particle *ppdev, const Force *ffdev) {
    Particle *pphst;
    Force *ffhst;
    char name[FILENAME_MAX];

    EMALLOC(n, &pphst);
    EMALLOC(n, &ffhst);

    cD2H(pphst, ppdev, n);
    cD2H(ffhst, ffdev, n);

    UC(gen_name(c, base, name));

    UC(txt_write_pp_ff(n, pphst, ffhst, name));

    EFREE(pphst);
    EFREE(ffhst);
}

static void print() {
    err_type e;
    UC(e = err_get());
    ERR("DBG: error: %s", get_err_str(e));
} 

void dbg_check_pos(const Coords *c, const char *base, const Dbg *dbg, int n, const Particle *pp) {
    int3 L = subdomain(c);
    if (!check(dbg, DBG_POS))
        return;
    UC(err_ini());
    KL(dbg_dev::check_pos, (k_cnf(n)), (L, pp, n));
    if (error()) {
        if (dbg->dump)
            dump_pp(c, base, n, pp);
        UC(print());
    }
}

void dbg_check_pos_soft(const Coords *c, const char *base, const Dbg *dbg, int n, const Particle *pp) {
    int3 L = subdomain(c);
    if (!check(dbg, DBG_POS_SOFT))
        return;
    UC(err_ini());
    KL(dbg_dev::check_pos_pu, (k_cnf(n)), (L, pp, n));
    if (error()) {
        if (dbg->dump)
            dump_pp(c, base, n, pp);
        UC(print());
    }
}

void dbg_check_vel(float dt, const Coords *c, const char *base, const Dbg *dbg, int n, const Particle *pp) {
    int3 L = subdomain(c);
    
    if (!check(dbg, DBG_VEL))
        return;
    UC(err_ini());
    KL(dbg_dev::check_vv, (k_cnf(n)), (dt, L, pp, n));
    if (error()) {
        if (dbg->dump)
            dump_pp(c, base, n, pp);
        UC(print());
    }
}

void dbg_check_forces(float dt, const Coords *c, const char *base, const Dbg *dbg, int n, const Particle *pp, const Force *ff) {
    int3 L = subdomain(c);
    
    if (!check(dbg, DBG_FORCES))
        return;
    UC(err_ini());
    KL(dbg_dev::check_ff, (k_cnf(n)), (dt, L, ff, n));
    if (error()) {
        if (dbg->dump)
            dump_pp_ff(c, base, n, pp, ff);
        UC(print());
    }
}

void dbg_check_colors(const Coords*, const Dbg *dbg, int n, const int *cc) {
    if (!check(dbg, DBG_COLORS))
        return;
    UC(err_ini());
    KL(dbg_dev::check_cc, (k_cnf(n)), (cc, n));
    if (error()) {
        UC(print());
    }
}

void dbg_check_clist(const Coords*, const Dbg *dbg, int3 L, const int *starts, const int *counts, const Particle *pp) {
    if (!check(dbg, DBG_CLIST))
        return;
    int n = L.x * L.y * L.z;
    UC(err_ini());
    KL(dbg_dev::check_clist, (k_cnf(n)), (L, starts, counts, pp));
    if (error()) {
        UC(print());
    }
}
