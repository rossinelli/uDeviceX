namespace rex {

namespace s {
void wait0() {
    MPI_Status s[26];
    MC(l::m::Waitall(26, reqsendP.data(), s));
}
}

namespace r {
void wait0() {
    MPI_Status s[26];
    MC(l::m::Waitall(26, reqrecvP.data(), s));
}
}

}
