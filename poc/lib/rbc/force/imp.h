struct RbcParams;
struct RbcForce;
struct Force;
struct Config;
struct MeshRead;
struct RbcQuants;

// tag::mem[]
void rbc_force_ini(const MeshRead*, RbcForce**);
void rbc_force_fin(RbcForce*);
// end::mem[]

// tag::set[]
void rbc_force_set_stressful(int nt, float totArea, /**/ RbcForce*); // <1>
void rbc_force_set_stressfree(const char *fname, /**/ RbcForce*);    // <2>

void rbc_force_set_rnd0(RbcForce*);           // <3>
void rbc_force_set_rnd1(int seed, RbcForce*); // <4>
void rbc_force_set_bending(const MeshRead*, const char*, RbcForce*);
// end::set[]

// tag::cnf[]
void rbc_force_set_conf(const MeshRead*, const Config *cfg, const char *name, RbcForce*);
// end::cnf[]

// tag::apply[]
void rbc_force_apply(RbcForce*, const RbcParams*, float dt, const RbcQuants*, /**/ Force*); // <1>
void rbc_force_stat(/**/ float *pArea, float *pVolume); // <2>
// end::apply[]
