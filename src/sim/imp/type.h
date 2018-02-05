/* types local for sim:: */

/* solvent distribution */
struct FluDistr {
    DFluPack *p;
    DFluComm *c;
    DFluUnpack *u;
    DFluStatus *s;
};

/* particle exchanger for solvent forces */
struct FluExch {
    EFluPack *p;
    EFluComm *c;
    EFluUnpack *u;
};

/* rbc distribution */
struct RbcDistr {
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

/* object exchanger for forces */
struct ObjExch {
    EObjPack *p;
    EObjUnpack *u;
    EObjPackF *pf;
    EObjUnpackF *uf;
    EObjComm *c;
};

/* mesh exchanger */
struct Mexch {
    EMeshPack *p;
    EMeshComm *c;
    EMeshUnpack *u;
};

/* bounce back exchanger */
struct BBexch : Mexch {
    EMeshPackM *pm;
    EMeshCommM *cm;
    EMeshUnpackM *um;
};

struct Colorer {
    Mexch e;                 /* mesh exchanger     */
    Particle *pp;            /* particle workspace */
    float3 *minext, *maxext; /* bounding boxes     */
};

/* holder for bounce back tools and work */
struct BounceBack {
    MeshBB *d;
    Momentum *mm;
    BBexch e;
};

/* data holder for solvent */
struct Flu {
    FluQuants q;
    PairParams *params;
    
    FluDistr d;
    FluExch e;

    FluForcesBulk *bulk;
    FluForcesHalo *halo;

    Force *ff;
    Force *ff_hst; /* solvent forces on host    */
};

/* data holder for red blood cells */
struct Rbc {
    RbcQuants q;
    RbcForce tt;

    RbcDistr d;

    Force *ff;

    RbcParams *params;    /* model parameters */
    RbcCom    *com;     /* helper to compute center of masses */
    RbcStretch *stretch;  /* helper to apply stretching [fo]rce to cells */
    OffRead    *cell;     /* cell template */
    MeshWrite  *mesh_write;
};

/* data holder for rigid objects */
struct Rig {
    RigQuants q;
    Scan *ws; /* work for scan */
    Force *ff, *ff_hst;

    RigDistr d;
    MeshWrite  *mesh_write;
};

/* data holder for walls */
struct Wall {
    Sdf *sdf;
    WallQuants q;
    WallTicket *t;
    Wvel *vel;
    Wvel_v vview;
};

/* helper for computing object interactions */
struct ObjInter {
    ObjExch e;
    Contact *cnt;
    Fsi     *fsi;

    PairParams *cntparams;
    PairParams *fsiparams;
};

/* velocity controller */
struct Vcon {
    PidVCont *vcont;
    int log_freq;
    int adjust_freq;
    int sample_freq;
};

/* optional features */
struct Opt {
    bool inflow, outflow, denoutflow, vcon;
    bool dump_field, dump_parts;
    int  freq_field, freq_parts;
};

struct Sim {
    /* quantities */
    Flu flu;
    Rbc rbc;
    Rig rig;
    Wall wall;

    /* parameters */
    MoveParams *moveparams;
    
    /* helpers */
    Coords *coords;
    int3 L;  /* subdomain sizes */
    ObjInter objinter;
    BounceBack bb;
    Colorer colorer;
    Vcon vcon;
    Restrain *restrain;
    Dbg *dbg;
    MPI_Comm cart;

    /* open bc tools */
    Outflow *outflow;
    Inflow *inflow;
    DCont    *denoutflow;
    DContMap *mapoutflow;

    /* particles on host for dump */
    Particle *pp_dump;
    BopWork *dumpt;

    /* runtime config */
    Config *cfg;

    /* state */
    bool solids0;
    bool equilibrating;

    Opt opt;
    float dt;

    /* inter processing helpers */
    GenColor *gen_color;
    TimeStep *time_step;
};
