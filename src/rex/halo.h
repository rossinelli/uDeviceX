namespace rex {
void halo() {
    ParticlesWrap halos[26];
    for (int i = 0; i < 26; ++i) halos[i] = ParticlesWrap(remote[i]->dstate.D, remote[i]->dstate.S, remote[i]->result.DP);

    dSync();
    if (fsiforces)     fsi::halo(halos);
    if (contactforces) cnt::halo(halos);
    for (int i = 0; i < 26; ++i) local[i]->update();
}
}
