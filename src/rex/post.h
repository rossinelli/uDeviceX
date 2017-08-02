namespace rex {
void post_count(x::TicketPack tp) {
    int i;
    for (i = 0; i < 26; ++i) send_counts[i] = tp.offsets_hst[i];
}

bool post_check() {
    bool packingfailed;
    int i;
    packingfailed = false;
    for (i = 0; i < 26; ++i) packingfailed |= send_counts[i] > local[i]->capacity();
    return packingfailed;
}

void post_resize() {
    int capacities[26];
    int *scattered_indices[26];
    int i;
    for (i = 0; i < 26; ++i) capacities[i] = local[i]->capacity();
    CC(cudaMemcpyToSymbolAsync(k_rex::g::capacities, capacities, sizeof(capacities), 0, H2D));
    for (i = 0; i < 26; ++i) scattered_indices[i] = local[i]->scattered_indices->D;
    CC(cudaMemcpyToSymbolAsync(k_rex::g::scattered_indices, scattered_indices, sizeof(scattered_indices), 0, H2D));
}

void local_resize() {
    int i;
    for (i = 0; i < 26; ++i) local[i]->resize(send_counts[i]);
}

void post_p(MPI_Comm cart, int dranks[26], x::TicketTags t, x::TicketPack tp) {
    if (tp.tstarts_hst[26])
        CC(cudaMemcpyAsync(host_packbuf, packbuf, sizeof(Particle) * tp.tstarts_hst[26], H2H));
    dSync();
    reqsendC.resize(26);
    for (int i = 0; i < 26; ++i)
        MC(l::m::Isend(send_counts + i, 1, MPI_INTEGER, dranks[i], t.btc + i, cart, &reqsendC[i]));

    for (int i = 0; i < 26; ++i) {
        int start = tp.tstarts_hst[i];
        int count = send_counts[i];
        int expected = local[i]->expected();
        
        MPI_Request reqP;
        MC(l::m::Isend(host_packbuf + start, expected * 6, MPI_FLOAT, dranks[i], t.btp1 + i, cart, &reqP));
        reqsendP.push_back(reqP);
        
        if (count > expected) {
            MPI_Request reqP2;
            MC(l::m::Isend(host_packbuf + start + expected, (count - expected) * 6, MPI_FLOAT, dranks[i], t.btp2 + i, cart, &reqP2));
            reqsendP.push_back(reqP2);
        }
    }
}
}
