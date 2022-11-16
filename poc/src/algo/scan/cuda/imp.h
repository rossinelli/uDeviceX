enum { THREADS = 128 };
static void scan0(const unsigned char *input, int size, /**/ uint *output, /*w*/ uint *tmp) {
    int nblocks = ((size / 16) + THREADS - 1 ) / THREADS;
    KL(scan_dev::breduce<THREADS/32>, (nblocks, THREADS                ), ((uint4 *)input, tmp, size / 16));
    KL(scan_dev::bexscan<THREADS>   , (1, THREADS, nblocks*sizeof(uint)), (tmp, nblocks));
    KL(scan_dev::gexscan<THREADS/32>, (nblocks, THREADS                ), ((uint4 *)input, tmp, (uint4 *)output, size / 16));
}

void scan_apply(const int *input, int size, /**/ int *output, /*w*/ Scan *w) {
    if (size > w->size)
        ERR(" size = %d > w->size = %d", size, w->size);
    if (!d::is_device_pointer(input))  ERR("`input`  is not a device pointer");
    if (!d::is_device_pointer(output)) ERR("`output` is not a device pointer");

    KL(scan_dev::compress, (k_cnf(size)), (size, (const int4*) input, /**/ (uchar4 *) w->compressed));
    scan0(w->compressed, size, /**/ (uint*) output, /*w*/ w->tmp);
}

void scan_ini(int size, /**/ Scan **work) {
    Scan *w;
    EMALLOC(1, work);
    w = *work;
    w->size = size;
    Dalloc(&w->tmp, 64 * 64 * 64 / THREADS);
    Dalloc(&w->compressed, 4 * size);
}

void scan_fin(/**/ Scan *w) {
    Dfree(w->tmp);
    Dfree(w->compressed);
    EFREE(w);
}
