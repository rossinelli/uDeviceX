struct PairParams;
struct FluForcesBulk;
struct FluForcesHalo;
struct PaArray;
struct FoArray;

// tag::memb[]
void fluforces_bulk_ini(int3 L, int maxp, /**/ FluForcesBulk **b);
void fluforces_bulk_fin(/**/ FluForcesBulk *b);
// end::memb[]

// tag::intb[]
void fluforces_bulk_prepare(int n, const PaArray *a, /**/ FluForcesBulk *b);
void fluforces_bulk_apply(const PairParams*, int n, const FluForcesBulk *b, const int *start, const int *count,
                          /**/ const FoArray *ff);
// end::intb[]

// tag::memh[]
void fluforces_halo_ini(MPI_Comm cart, int3 L, /**/ FluForcesHalo **hd);
void fluforces_halo_fin(/**/ FluForcesHalo *h);
// end::memh[]

// tag::inth[]
void fluforces_halo_prepare(flu::LFrag26 lfrags, flu::RFrag26 rfrags, /**/ FluForcesHalo *h);
void fluforces_halo_apply(const PairParams*, const FluForcesHalo *h, /**/ const FoArray *farray);
// end::inth[]

