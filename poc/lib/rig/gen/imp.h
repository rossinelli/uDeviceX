struct Coords;
struct RigPinInfo;
struct MeshRead;

// tag::struct[]
struct RigGenInfo {
    float mass;
    int numdensity;
    const RigPinInfo *pi;
    int nt, nv;
    const int4 *tt;     /* on device */
    const Particle *pp; /* on device */
    bool empty_pp;
    const MeshRead *mesh;
};

struct FluInfo {
    Particle *pp;
    int *n;
};

/* everything on host */
struct RigInfo {
    int ns, *nps;
    float *rr0;
    Rigid *ss;
    Particle *pp;
};
// end::struct[]

// tag::int[]
void rig_gen_from_solvent(const Coords*, MPI_Comm, RigGenInfo, /*io*/ FluInfo, /**/ RigInfo);
// end::int[]
