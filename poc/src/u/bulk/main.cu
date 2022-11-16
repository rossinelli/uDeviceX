#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <conf.h>
#include "inc/conf.h"

#include "d/api.h"
#include "utils/msg.h"
#include "utils/error.h"
#include "utils/imp.h"
#include "utils/cc.h"
#include "utils/mc.h"

#include "mpi/glb.h"
#include "mpi/wrapper.h"
#include "inc/dev.h"
#include "inc/type.h"
#include "conf/imp.h"
#include "struct/partlist/type.h"
#include "clist/imp.h"
#include "pair/imp.h"

#include "coords/ini.h"
#include "coords/imp.h"

#include "struct/parray/imp.h"
#include "struct/farray/imp.h"
#include "flu/type.h"
#include "fluforces/imp.h"

#include "io/txt/imp.h"

static Particle *pp, *pp0, *pp_hst;
static Force *ff, *ff_hst;
static int n;
static Clist *clist;
static ClistMap *cmap;
static FluForcesBulk *bulkforces;

static void read_pp(const char *fname) {
    TxtRead *tr;
    long sz;
    UC(txt_read_pp(fname, &tr));
    n = txt_read_get_n(tr);
    msg_print("have read %d particles", n);

    sz = n + 32;

    EMALLOC(sz, &pp_hst);
    EMALLOC(sz, &ff_hst);

    Dalloc(&pp,  sz);
    Dalloc(&pp0, sz);
    Dalloc(&ff,  sz);

    memcpy(pp_hst, txt_read_get_pp(tr), sz * sizeof(Particle));
    CC(d::Memcpy(pp, pp_hst, sz * sizeof(Particle), H2D));
    CC(d::Memset(ff, 0, sz * sizeof(Force)));

    UC(txt_read_fin(tr));
}

static void dealloc() {
    Dfree(pp);
    Dfree(pp0);
    Dfree(ff);
    EFREE(pp_hst);
    EFREE(ff_hst);
    n = 0;
}

static void build_clist() {
    UC(clist_build(n, n, pp, /**/ pp0, clist, cmap));
    Particle *tmp = pp;
    pp = pp0;
    pp0 = tmp;
}

static void set_params(const Config *cfg, float dt, PairParams *p) {
    float kBT;
    UC(conf_lookup_float(cfg, "glb.kBT", &kBT));
    msg_print("kBT: %g", kBT);
    UC(pair_set_conf(cfg, "flu", p));
    UC(pair_compute_dpd_sigma(kBT, dt, /**/ p));
}

int main(int argc, char **argv) {
    Config *cfg;
    const char *fin, *fout;
    PaArray parray;
    FoArray farray;
    Coords *coords;
    int maxp;
    int3 L;
    PairParams *params;
    float dt;
    int rank, dims[3];
    MPI_Comm cart;
    
    m::ini(&argc, &argv);
    m::get_dims(&argc, &argv, dims);
    m::get_cart(MPI_COMM_WORLD, dims, &cart);
    
    MC(m::Comm_rank(cart, &rank));
    msg_ini(rank);

    UC(conf_ini(&cfg));
    UC(conf_read(argc, argv, cfg));

    UC(coords_ini_conf(cart, cfg, &coords));
    L = subdomain(coords);

    UC(pair_ini(&params));
    UC(conf_lookup_float(cfg, "time.dt", &dt));
    UC(set_params(cfg, dt, params));

    UC(conf_lookup_string(cfg, "in", &fin));
    UC(conf_lookup_string(cfg, "out", &fout));
    UC(read_pp(fin));

    maxp = n + 32;

    UC(clist_ini(L.x, L.y, L.z, &clist));
    UC(clist_ini_map(maxp, 1, clist, &cmap));
    UC(build_clist());

    UC(fluforces_bulk_ini(L, maxp, &bulkforces));

    parray_push_pp(pp, &parray);
    farray_push_ff(ff, &farray);

    UC(fluforces_bulk_prepare(n, &parray, /**/ bulkforces));
    UC(fluforces_bulk_apply(params, n, bulkforces, clists_get_ss(clist), clists_get_cc(clist), /**/ &farray));

    // particles are reordered because of clists
    CC(d::Memcpy(pp_hst, pp, n*sizeof(Particle), D2H));
    CC(d::Memcpy(ff_hst, ff, n*sizeof(Force)   , D2H));
    UC(txt_write_pp_ff(n, pp_hst, ff_hst, fout));

    UC(fluforces_bulk_fin(bulkforces));
    UC(clist_fin(clist));
    UC(clist_fin_map(cmap));
    UC(dealloc());

    UC(pair_fin(params));
    UC(coords_fin(coords));
    UC(conf_fin(cfg));

    MC(m::Barrier(cart));
    m::fin();
}
