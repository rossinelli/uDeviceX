static float ff(long it, long nsteps) {
    float gd = 0;
    int i, n = 8;
    for (i = 1; i <= n; i++)
        if (n*it < i*nsteps) {
            gd =  2.0*i/n*WVEL_PAR_A;
            break;
        }
    return gd;
}

static void report0(long i, long e, float gd) {
    MSG("VWALL_DUPIRE_UP: gd = %6.2g : step %07ld/%07ld", gd, i, e);
}
