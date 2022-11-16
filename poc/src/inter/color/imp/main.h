void inter_color_ini(GenColor **c) {
    GenColor *gc;
    EMALLOC(1, c);
    gc = *c;
    gc->kind = NONE;
}

void inter_color_fin(GenColor *c) {
    EFREE(c);
}

void inter_color_set_drop(float R, GenColor *c) {
    c->kind = DROP;
    c->R = R;
}

void inter_color_set_uniform(GenColor *c) {
    c->kind = UNIF;
}

void inter_color_apply_hst(const Coords *coords, const GenColor *gc, int n, const Particle *pp, /**/ int *cc) {
    switch (gc->kind) {
    case UNIF:
        set_color_unif(n, /**/ cc);
        break;
    case DROP:
        set_color_drop(gc->R, coords, n, pp, /**/ cc);
        break;
    case NONE:
        break;
    default:
        ERR("Unrecognised kind <%d>", gc->kind);
        break;
    };
}

void inter_color_apply_dev(const Coords *coords, const GenColor *gc, int n, const Particle *pp, /**/ int *cc) {
    int *cc_hst;
    Particle *pp_hst;

    if (gc->kind == NONE) return;
    
    EMALLOC(n, &cc_hst);
    EMALLOC(n, &pp_hst);

    cD2H(pp_hst, pp, n);

    UC(inter_color_apply_hst(coords, gc, n, pp_hst, /**/ cc_hst));

    cH2D(cc, cc_hst, n);
    
    EFREE(pp_hst);
    EFREE(cc_hst);
}
