int emalloc(size_t, /**/ void **);
void efree(void*);

int efopen(const char *fname, const char *mode, /**/ FILE **);
void efclose(FILE *);
