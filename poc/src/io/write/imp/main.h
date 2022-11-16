int write_file_open(MPI_Comm comm, const char *fn, /**/ WriteFile **pq) {
    WriteFile *q;
    EMALLOC(1, &q);
    MC(m::File_open(comm, fn, MPI_MODE_WRONLY |  MPI_MODE_CREATE, MPI_INFO_NULL, &q->f));
    MC(m::File_set_size(q->f, 0));
    *pq = q;
    return 0;
}

int write_file_close(WriteFile *fp) {
    MPI_File f;
    f = fp->f;
    MC(m::File_close(&f));
    EFREE(fp);
    return 0;
}

void write_all(MPI_Comm comm, const void * const ptr, int nbytes32, WriteFile *fp) {
    MPI_File f = fp->f;
    MPI_Offset base;
    MPI_Offset offset = 0, nbytes = nbytes32;
    MPI_Status status;
    MPI_Offset ntotal = 0;
    MC(m::File_get_position(f, &base));
    MC(m::Exscan(&nbytes, &offset, 1, MPI_OFFSET, MPI_SUM, comm));
    MC(m::File_write_at_all(f, base + offset, ptr, nbytes, MPI_CHAR, &status));
    MC(m::Allreduce(&nbytes, &ntotal, 1, MPI_OFFSET, MPI_SUM, comm) );
    MC(m::File_seek(f, ntotal, MPI_SEEK_CUR));
}

int write_master(MPI_Comm comm, const void * const ptr, int sz0, WriteFile *fp) {
    int sz;
    sz = m::is_master(comm) ? sz0 : 0;
    write_all(comm, ptr, sz, fp);
    return 0;
}

int write_shift_indices(MPI_Comm comm, int n, int *shift0) {
    *shift0 = 0;
    MC(m::Exscan(&n, shift0, 1, MPI_INTEGER, MPI_SUM, comm));
    return 0;
}

int write_reduce(MPI_Comm comm, int n0, /**/ int *n) {
    *n = 0;
    MC(m::Reduce(&n0, n, 1, MPI_INT, MPI_SUM, 0, comm));
    return 0;
}
