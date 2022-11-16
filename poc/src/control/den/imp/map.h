struct Circle {
    float R;    
};

struct Plate {
    // TODO
};

struct None {};

static int predicate(const Coords *c, Circle p, int i, int j, int k) {
    enum {X, Y, Z};
    // TODO: for now assume centered at the center of the domain
    float3 r;
    float R;

    r = make_float3(i + (1 - xs(c)) * 0.5f,
                    j + (1 - ys(c)) * 0.5f,
                    k + (1 - zs(c)) * 0.5f);
    local2center(c, r, /**/ &r);
    
    R = sqrt(r.x * r.x + r.y * r.y);
    return R < p.R && R >= p.R - 1;
}

static int predicate(const Coords*, None, int, int, int) {
    return 0;
}

template <typename T>
static void ini_map(const Coords *coords, T p, int **ids, int *nids) {
    int *ii, i, j, k, n;
    size_t sz;
    int Lx, Ly, Lz;
    Lx = xs(coords);  Ly = ys(coords);  Lz = zs(coords);
    
    EMALLOC(Lx * Ly * Lz, &ii);

    n = 0;
    for (k = 0; k < Lz; ++k) {
        for (j = 0; j < Ly; ++j) {
            for (i = 0; i < Lx; ++i) {
                if (predicate(coords, p, i, j, k))
                    ii[n++] = i + Lx * (j + Ly * k);
            }
        }
    }
    
    *nids = n;
    sz = n * sizeof(int);
    
    Dalloc(ids, n);
    CC(d::Memcpy(*ids, ii, sz, H2D));
    
    EFREE(ii);
}

void den_map_ini(DContMap **m) {
    EMALLOC(1, m);
    (*m)->cids = NULL;
}

void den_map_set_none(const Coords *c, DContMap *m) {
    None p;
    ini_map(c, p, &m->cids, &m->n);
}

void den_map_set_circle(const Coords *c, float R, DContMap *m) {
    Circle p;
    p.R = R;
    ini_map(c, p, &m->cids, &m->n);
}

void den_map_fin(DContMap *m) {
    if (m->cids) Dfree(m->cids);
    EFREE(m);
}
