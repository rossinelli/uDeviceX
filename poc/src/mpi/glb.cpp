#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include <conf.h>
#include "inc/conf.h"

#include "mpi/wrapper.h"
#include "inc/conf.h"
#include "utils/mc.h"

namespace m { /* MPI */
enum {X, Y, Z, D};

void ini(int *argc, char ***argv) {
    if (m::Init(argc, argv) != MPI_SUCCESS) {
        fprintf(stderr, ": m::Init failed\n");
        exit(2);
    }
    if (m::Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN) != MPI_SUCCESS) {
        fprintf(stderr, ": m::Errhandler_set failed\n");
        exit(2);
    }
}

static void shift(int *argc, char ***argv) {
    (*argc)--;
    (*argv)++;
}
static int eq(const char *a, const char *b) { return strcmp(a, b) == 0; }

void get_dims(int *argc, char ***argv, int dims[]) {
    int i, d, ac;
    char **av;

    // defaults
    dims[X] = dims[Y] = dims[Z] = 1;
    ac = *argc; av = *argv;

    // skip executable
    shift(&ac, &av);

    for (i = 0; ac > 0 && i <= D; i++) {
        if (eq(av[0], "--")) {
            shift(&ac, &av);
            break;
        }
        if ( (d = atoi(av[0])) == 0 ) break;
        dims[i] = d;
        shift(&ac, &av);
    }

    *argc = ac; *argv = av;
}

void get_cart(MPI_Comm comm, const int dims[3], /**/ MPI_Comm *cart) {
    enum {X, Y, Z};
    const int reorder = 0;
    const int periods[D] = {1, 1, 1};
    int size;
    MC(m::Comm_size(comm, &size));
    if (size != dims[X]*dims[Y]*dims[Z]) {
        fprintf(stderr, "%s:%d: fail to create cartesian geometry\n", __FILE__, __LINE__);
        fprintf(stderr, "size=%d  !=  dims[X]=%d  * dims[Y]=%d  * dims[Z]=%d\n",
                size, dims[X], dims[Y], dims[Z]);
        exit(2);
    }
    MC(m::Cart_create(comm, D, dims, periods, reorder, cart));
}

void fin() { MC(m::Finalize()); }

} /* namespace */
