void comm_post_recv(hBags *b, Comm *com) {
    int i, c, tag;
    for (i = 0; i < NFRAGS; ++i) {
        c = b->capacity[i] * b->bsize;
        tag = com->tags[i];
        MC(m::Irecv(b->data[i], c, MPI_BYTE, com->ranks[i], tag, com->cart, com->rreq + i));
    }
}

static void fail_over(int i, long c, long cap) {
    enum {X, Y, Z};
    int d[3];
    frag_hst::i2d3(i, d);
    ERR("over capacity, fragment %d = [%d %d %d]: %ld/%ld",
        i, d[X], d[Y], d[Z], c, cap);
}

void comm_post_send(const hBags *b, Comm *com) {
    int i, n, c, cap, tag;
    for (i = 0; i < NFRAGS; ++i) {
        c = b->counts[i];
        cap = b->capacity[i];
        n = c * b->bsize;
        tag = i;
        if (c > cap) UC(fail_over(i, c, cap));
        MC(m::Isend(b->data[i], n, MPI_BYTE, com->ranks[i], tag, com->cart, com->sreq + i));
    }
}

static void get_counts_bytes(const MPI_Status ss[NFRAGS], /**/ int counts[NFRAGS]) {
    for (int i = 0; i < NFRAGS; ++i)
        MC(m::Get_count(ss + i, MPI_BYTE, counts + i));
}

static void get_counts(const MPI_Status ss[NFRAGS], /**/ hBags *b) {
    int cap, i, c, cc[NFRAGS];
    get_counts_bytes(ss, /**/ cc);

    for (i = 0; i < NFRAGS; ++i) {
        c = cc[i] / b->bsize;
        b->counts[i] = c;
        cap = b->capacity[i];
        if (c > cap) UC(fail_over(i, c, cap));
    }
}

static void get_counts(const MPI_Status ss[NFRAGS], /**/ CommBuffer *b) {
    int i, cc[NFRAGS];
    size_t cap, c;
    get_counts_bytes(ss, /**/ cc);

    for (i = 0; i < NFRAGS; ++i) {
        c = cc[i];
        cap = b->cap[i];
        b->sz[i] = c;
        if (c > cap) UC(fail_over(i, c, cap));
    }
}

static void fail_wait_normal(int code) {
    int sz;
    char msg[BUFSIZ];
    m::Error_string(code, msg, &sz);
    ERR(msg);
}

static void fail_wait_status(int n, MPI_Status *ss) {
    /* get error message from status */
    enum {X, Y, Z};
    int i, sz, code, d[3];
    char msg[BUFSIZ];
    for (i = 0; i < n; i++) {
        code = m::status2errcode(&ss[i]);
        if (m::is_success(code) || m::is_pending(code)) continue;
        frag_hst::i2d3(i, d);
        m::Error_string(code, msg, &sz);
        ERR("mpi error in fragment %d = [%d %d %d], %s", i,
            d[X], d[Y], d[Z], msg);
    }
    ERR("assert");
}

static void fail_wait(int code, int n, MPI_Status *ss) {
    if (m::is_err_in_status(code)) UC(fail_wait_status(n, ss));
    else  UC(fail_wait_normal(code));
}

void comm_wait_recv(Comm *com, /**/ hBags *b) {
    int errorcode;
    MPI_Status ss[NFRAGS];
    errorcode = m::Waitall(NFRAGS, com->rreq, /**/ ss);
    if (!m::is_success(errorcode)) fail_wait(errorcode, NFRAGS, ss);
    get_counts(ss, /**/ b);
}

void comm_post_recv(CommBuffer *cb, Comm *com) {
    int i, tag;
    for (i = 0; i < NFRAGS; ++i) {
        tag = com->tags[i];
        MC(m::Irecv(cb->buf[i], cb->cap[i], MPI_BYTE, com->ranks[i], tag, com->cart, com->rreq + i));
    }
}

void comm_post_send(const CommBuffer *cb, Comm *com) {
    int i, tag;
    long c, cap;
    for (i = 0; i < NFRAGS; ++i) {
        c = cb->sz[i];
        cap = cb->cap[i];
        tag = i;
        if (c > cap) UC(fail_over(i, c, cap));
        MC(m::Isend(cb->buf[i], c, MPI_BYTE, com->ranks[i], tag, com->cart, com->sreq + i));
    }
}
    

void comm_wait_recv(Comm *com, /**/ CommBuffer *cb) {
    int errorcode;
    MPI_Status ss[NFRAGS];
    errorcode = m::Waitall(NFRAGS, com->rreq, /**/ ss);
    if (!m::is_success(errorcode)) fail_wait(errorcode, NFRAGS, ss);
    get_counts(ss, /**/ cb);
}

void comm_wait_send(Comm *com) {
    MPI_Status ss[NFRAGS];
    MC(m::Waitall(NFRAGS, com->sreq, ss));
}



int comm_get_number_capacity(int i, const hBags *b) {
    return b->capacity[i];
}

size_t comm_get_byte_capacity(int i, const hBags *b) {
    return b->bsize * comm_get_number_capacity(i, b);
}
