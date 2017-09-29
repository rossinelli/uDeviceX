static void bind0(const int *const starts, const int *const cellentries,
                  const int nc, int nw, PaWrap *pw, FoWrap *fw) {
    size_t textureoffset;
    int ncells, i;
    float2 *ps[MAX_OBJ_TYPES];
    float *fs[MAX_OBJ_TYPES];

    textureoffset = 0;
    if (nc)
        CC(cudaBindTexture(&textureoffset, &dev::c::id, cellentries,
                           &dev::c::id.channelDesc,
                           sizeof(int) * nc));
    ncells = XS * YS * ZS;
    CC(cudaBindTexture(&textureoffset, &dev::c::start, starts,
                       &dev::c::start.channelDesc, sizeof(int) * ncells));

    assert(nw <= MAX_OBJ_TYPES);
    for (i = 0; i < nw; ++i) {
        ps[i] = (float2*)pw[i].pp;
        fs[i] = (float*)fw[i].ff;
    }

    CC(cudaMemcpyToSymbolAsync(dev::c::csolutes, ps, sizeof(float2*)*nw, 0, H2D));
    CC(cudaMemcpyToSymbolAsync(dev::c::csolutesacc, fs, sizeof(float*)*nw, 0, H2D));
}
