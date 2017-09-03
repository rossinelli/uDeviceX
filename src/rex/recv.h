namespace rex {
void recvF(MPI_Comm cart, int ranks[26], int tags[26], x::TicketTags t, int counts[26]) {
    int i, count, tag;
    MPI_Request reqA;
    for (i = 0; i < 26; ++i) {
        tag = t.btf + tags[i];
        count = counts[i];
        MC(m::Irecv(local[i].ff_pi, 3 * count, MPI_FLOAT, ranks[i], tag, cart, &reqA));
        reqrecvA.push_back(reqA);
    }
}

void recvC(MPI_Comm cart, int ranks[26], int tags[26], x::TicketTags t, int counts[26]) {
    int i, tag;
    MPI_Request reqC;
    for (i = 0; i < 26; ++i) {
        tag = t.btc + tags[i];
        MC(m::Irecv(counts + i, 1, MPI_INTEGER, ranks[i], tag, cart, &reqC));
        reqrecvC.push_back(reqC);
    }
}

void recvP(MPI_Comm cart, int ranks[26], int tags[26], x::TicketTags t, int counts[26], RFrag *remote) {
    int i, tag, n;
    MPI_Request reqP;
    Particle *p;
    for (i = 0; i < 26; ++i) {
        tag = t.btp1 + tags[i];
        n = counts[i];
        p = remote[i].pp_pi;
        MC(m::Irecv(p, n * 6, MPI_FLOAT, ranks[i], tag, cart, &reqP));
        reqrecvP.push_back(reqP);
    }
}
}
