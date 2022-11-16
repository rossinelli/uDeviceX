struct Clist;
struct ClistMap;

// tag::mem[]
void clist_ini(int LX, int LY, int LZ, /**/ Clist **c);
void clist_fin(/**/ Clist *c);

void clist_ini_map(int maxp, int nA, const Clist *c, /**/ ClistMap **m);
void clist_fin_map(ClistMap *m);
// end::mem[]

// tag::access[]
const int* clists_get_ss(const Clist *c);
const int* clists_get_cc(const Clist *c);
int3       clists_get_dim(const Clist *c);
int        clists_get_n(const Clist *c);
const uint* clist_get_ids(const ClistMap *m);
// end::access[]

// tag::int[]
void clist_ini_counts(Clist *c);
void clist_subindex(bool project, int aid, int n, const PartList lp, /**/ Clist *c, ClistMap *m);
void clist_build_map(const int nn[], /**/ Clist *c, ClistMap *m);

/* special for fluid distribution */
void clist_subindex_local(int n, const PartList lp, /**/ Clist *c, ClistMap *m);
void clist_subindex_remote(int n, const PartList lp, /**/ Clist *c, ClistMap *m);

void clist_gather_pp(const Particle *pplo, const Particle *ppre, const ClistMap *m, long nout, /**/ Particle *ppout);
void clist_gather_ii(const int *iilo, const int *iire, const ClistMap *m, long nout, /**/ int *iiout);

/* quick cell build for single array */
void clist_build(int nlo, int nout, const Particle *pplo, /**/ Particle *ppout, Clist *c, ClistMap *m);
// end::int[]
