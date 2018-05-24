// tag::struct[]
enum {
    MAX_NHBAGS = 2,
    MAX_NDBAGS = 1
};

struct DRbcPack {
    DMap map;
    float3 *minext, *maxext;
    dBags dbags[MAX_NDBAGS], dpp;
    hBags hbags[MAX_NHBAGS], hpp, hii;

    /* optional: ids */
    bool ids;
    DMap hmap;

    int3 L; /* subdomain size */
};

struct DRbcComm {
    /* optional: ids */
    Comm *pp, *ii;
    bool ids;
};

struct DRbcUnpack {
    hBags hbags[MAX_NHBAGS], hpp, hii;

    /* optional: ids */
    bool ids;

    int3 L; /* subdomain size */
};
// end::struct[]
