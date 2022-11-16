#include <stdio.h>
#include <mpi.h>

#include <vector_types.h>

#include "utils/imp.h"
#include "utils/error.h"
#include "conf/imp.h"

#include "ini.h"

void coords_ini_conf(MPI_Comm cart, const Config *cfg, Coords **c) {
    int3 L;
    UC(conf_lookup_int3(cfg, "glb/L", &L));
    UC(coords_ini(cart, L.x, L.y, L.z, c));
}
