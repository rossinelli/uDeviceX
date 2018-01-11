namespace distr {
namespace flu {

using namespace comm;

// tag::struct[]
struct Pack {
    DMap map;
    dBags dpp, dii, dcc;
    hBags hpp, hii, hcc;
    int nhalo; /* number of sent particles */
};

struct Comm {
    comm::Comm pp, ii, cc;
};

struct Unpack {
    hBags hpp, hii, hcc;
    Particle *ppre;
    int *iire, *ccre;
    int nhalo; /* number of received particles */
};
// end::struct[]

} // flu
} // distr
