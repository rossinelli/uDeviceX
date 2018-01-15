#include <stdio.h>

#include "utils/msg.h"
#include "mpi/glb.h"
#include "utils/error.h"
#include "parser/imp.h"

enum {MAX_VEC=128};

static void set(Config *c) {
    {
        const char *desc[] = {"group", "a"};
        conf_set_int(2, desc, 0, c);
    }

    {
        const char *desc[] = {"group", "subgroup", "a"};
        conf_set_int(3, desc, 5, c);
    }

    {
        const char *desc[] = {"a"};
        conf_set_int(1, desc, 0, c);
    }
}

int main(int argc, char **argv) {
    Config *cfg;
    m::ini(&argc, &argv);

    conf_ini(/**/ &cfg);
    conf_read(argc, argv, /**/ cfg);

    UC(set(cfg));
    conf_write_exe(cfg, stdout);
    
    conf_fin(/**/ cfg);    

    m::fin();
}
