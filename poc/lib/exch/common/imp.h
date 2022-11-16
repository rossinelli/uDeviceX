struct int3;

// tag::struct[]
struct PackHelper {
    int *starts;
    int *offsets;
    int *indices[NFRAGS];
    int *cap;
};
// end::struct[]

// tag::int[]
void ecommon_pack_pp(const Particle *pp, PackHelper ph, /**/ Pap26 buf); // <1>
void ecommon_shift_pp_one_frag(int3 L, int n, const int fid, /**/ Particle *pp); // <2>
void ecommon_shift_rrcp_one_frag(int3 L, int n, const int fid, /**/ Positioncp *rr);
// end::int[]
