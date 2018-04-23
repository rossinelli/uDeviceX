struct Wall;

struct OptParams;
struct Config;
struct PairParams;
struct Coords;

void wall_ini(const Config*, int3 L, Wall**);
void wall_fin(Wall*);

void wall_gen(MPI_Comm, const Coords*, OptParams, bool dump_sdf,
              /*io*/ int *n, Particle *pp, /**/ Wall*);

void wall_restart(MPI_Comm, const Coords*, OptParams, bool dump_sdf,
                  const char *base, /**/ Wall*);
void wall_dump_templ(const Wall*, MPI_Comm, const char *base);

void wall_interact(const Coords*, const PairParams*, Wall*, PFarrays*);
void wall_bounce(const Wall*, const Coords*, float dt, PFarrays*);

void wall_update_vel(float t, Wall *);

void wall_get_sdf_ptr(const Wall*, const Sdf**);
double wall_compute_volume(const Wall*, MPI_Comm, int3 L);