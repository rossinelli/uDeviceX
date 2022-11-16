#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include <conf.h>
#include "inc/conf.h"

#include "algo/force_stat/imp.h"
#include "d/api.h"
#include "inc/dev.h"
#include "inc/type.h"
#include "io/txt/imp.h"
#include "mpi/glb.h"
#include "conf/imp.h"
#include "conf/imp.h"
#include "utils/cc.h"
#include "utils/mc.h"
#include "utils/error.h"
#include "utils/imp.h"
#include "utils/msg.h"
#include "mpi/wrapper.h"

static const char *prog_name = "./udx";
void usg0() {
    fprintf(stderr, "%s i=\\\"force.txt\\\"\n", prog_name);
    fprintf(stderr, "force.txt: f0x f0y f0z\n");
    fprintf(stderr, "           ...\n");
    fprintf(stderr, "           fnx fny fnz\n");
    fprintf(stderr, "dumps forces statistics\n");
    exit(0);
}

void usg(int c, char **v) {
    if (c > 1 && same_str(v[1], "-h"))
        usg0();
}

void main0(int n, const Force *hst) {
    float max;
    Force *dev;
    Dalloc(&dev, n);
    cH2D(dev, hst, n);
    UC(max = force_stat_max(n, dev));
    printf("%g\n", max);
    Dfree(dev);
}

int main(int argc, char **argv) {
    const char *i; /* input file */
    const Force *ff;
    int n, rank;
    Config *cfg;
    TxtRead *txt;
    MPI_Comm cart;
    int dims[3];

    usg(argc, argv);

    m::ini(&argc, &argv);
    m::get_dims(&argc, &argv, dims);
    m::get_cart(MPI_COMM_WORLD, dims, &cart);

    MC(m::Comm_rank(cart, &rank));
    msg_ini(rank);
    UC(conf_ini(&cfg));
    UC(conf_read(argc, argv, /**/ cfg));

    UC(conf_lookup_string(cfg, "i", &i));
    UC(txt_read_ff(i, &txt));

    ff = txt_read_get_ff(txt);
    n  = txt_read_get_n(txt);
    UC(main0(n, ff));

    UC(txt_read_fin(txt));
    UC(conf_fin(cfg));

    MC(m::Barrier(cart));
    m::fin();
}
