namespace x {

static void ini_ticketcom(MPI_Comm cart, /**/ int ranks[26]) {
    enum {X, Y, Z};
    int i, c;
    int ne[3], d[3];
    for (i = 0; i < 26; ++i) {
        d[X] = (i     + 2) % 3 - 1;
        d[Y] = (i / 3 + 2) % 3 - 1;
        d[Z] = (i / 9 + 2) % 3 - 1;
        for (c = 0; c < 3; ++c) ne[c] = m::coords[c] + d[c];
        MC(l::m::Cart_rank(cart, ne, ranks + i));
    }
}

void ini(/*io*/ basetags::TagGen *tg) {
    MC(l::m::Comm_dup(m::cart, &cart));
    ini_ticketcom(cart, dstranks);
    rex::ini(cart, tg);
}

void fin() {
    rex::fin();
    MC(l::m::Comm_free(&cart));
}

static void rex0(std::vector<ParticlesWrap> w, int nw) {
    rex::pack_p(nw);
    rex::_pack_attempt(w);
    rex::post_p(cart, dstranks, w);
    rex::recv_p(cart, dstranks, w);
    rex::halo(); /* fsi::halo(); */
    rex::_postrecvP(cart, dstranks);
    rex::post_f(cart, dstranks, w);
    rex::recv_f(w);
}

void rex(std::vector<ParticlesWrap> w) {
    int nw;
    nw = w.size();
    if (nw) rex0(w, nw);
}

}
