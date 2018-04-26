/* membrane distribution */
struct MbrDistr {
    DRbcPack *p;
    DRbcComm *c;
    DRbcUnpack *u;
};

/* rigid distribution */
struct RigDistr {
    DRigPack *p;
    DRigComm *c;
    DRigUnpack *u;
};

/* mesh exchanger */
struct MeshExch {
    EMeshPack *p;
    EMeshComm *c;
    EMeshUnpack *u;
};

/* mesh momentum exchanger */
struct MeshMomExch {
    EMeshPackM *p;
    EMeshCommM *c;
    EMeshUnpackM *u;
};

// struct Colorer {
//     Particle *pp;            /* particle workspace */
//     float3 *minext, *maxext; /* bounding boxes     */
// };

// struct BounceBack {
//     MeshBB *d;
//     Momentum *mm;
// };

/* data holder for cell membranes */
struct Mbr {
    RbcQuants q;
    MbrDistr d;
    Force *ff;
    RbcForce   *force;    /* helper to compute membrane forces         */
    RbcParams  *params;   /* model parameters                          */
    RbcCom     *com;      /* helper to compute center of masses        */
    RbcStretch *stretch;  /* helper to apply stretching force to cells */
    Triangles  *tri;      /* triangles for one cell on devices         */
    float mass;           /* mass of one particle                      */
    MeshRead   *mesh;     /* cell template                             */
    MeshWrite  *mesh_write;
};

/* data holder for rigid objects */
struct Rig {
    RigQuants q;
    Force *ff, *ff_hst;

    RigPinInfo *pininfo;
    RigDistr d;
    
    float mass;  /* mass of one particle */

    MeshRead   *mesh;
    MeshWrite  *mesh_write;
};

struct Dump {
    Particle *pp; /* workspace on host */
    long id;
};

struct Objects {
    Mbr *mbr;
    Rig *rig;
    Opt opt;
    Dump *dump;
    Coords *coords;
    MPI_Comm cart;
};
