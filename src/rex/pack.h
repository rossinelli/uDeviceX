namespace rex {
void clear(int nw, x::TicketPack tp) {
    CC(cudaMemsetAsync(tp.starts,  0, sizeof(int) * 27 *  nw));
    CC(cudaMemsetAsync(tp.counts,  0, sizeof(int) * 26 *  nw));
    CC(cudaMemsetAsync(tp.offsets, 0, sizeof(int) * 26 * (nw + 1)));
}

void pack(std::vector<ParticlesWrap> w, int nw, x::TicketPack tp, Particle *packbuf) {
    int i, n;
    int *o, *c, *s;
    CC(cudaMemcpyToSymbolAsync(k_rex::g::tstarts, tp.tstarts, sizeof(int) * 27, 0, D2D));
    for (i = 0; i < nw; ++i) {
        n = w[i].n;
        if (n) {
            const Particle *pp = w[i].p;
            o = tp.offsets + 26 *  i;
            c = tp.counts  + 26 *  i;
            s = tp.starts  + 27 *  i;
            CC(cudaMemcpyToSymbolAsync(k_rex::g::offsets, o, sizeof(int) * 26, 0, D2D));
            CC(cudaMemcpyToSymbolAsync(k_rex::g::counts,  c, sizeof(int) * 26, 0, D2D));
            CC(cudaMemcpyToSymbolAsync(k_rex::g::starts,  s, sizeof(int) * 27, 0, D2D));
            k_rex::pack<<<14 * 16, 128>>>((float2*)pp, /**/ (float2*)packbuf);
        }
    }
}

}
