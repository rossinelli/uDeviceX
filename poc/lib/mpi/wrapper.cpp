#include <mpi.h>
#include "wrapper.h"

namespace m {
int Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
    return MPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
}

int Barrier(MPI_Comm comm) {
    return MPI_Barrier(comm);
}

int Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm) {
    return MPI_Bcast(buffer, count, datatype, root, comm);
}

int Cancel(MPI_Request *request) {
    return MPI_Cancel(request);
}

int Cart_create(MPI_Comm comm_old, int ndims, const int dims[],
                const int periods[], int reorder, MPI_Comm *comm_cart) {
    return MPI_Cart_create(comm_old, ndims, dims, periods, reorder, comm_cart);
}

int Cart_coords(MPI_Comm comm, int rank, int maxdims, int coords[]) {
    return MPI_Cart_coords(comm, rank, maxdims, coords);
}

int Cart_get(MPI_Comm comm, int maxdims, int dims[], int periods[], int coords[]) {
    return MPI_Cart_get(comm, maxdims, dims, periods, coords);
}

int Cart_rank(MPI_Comm comm, const int coords[], int *rank) {
    return MPI_Cart_rank(comm, coords, rank);
}

int Comm_dup(MPI_Comm comm, MPI_Comm *newcomm) {
    return MPI_Comm_dup(comm, newcomm);
}

int Comm_free(MPI_Comm *comm) {
    return MPI_Comm_free(comm);
}

int Comm_rank(MPI_Comm comm, int *rank) {
    return MPI_Comm_rank(comm, rank);
}

int Comm_size( MPI_Comm comm, int *size ) {
    return MPI_Comm_size(comm, size);
}

int Errhandler_set(MPI_Comm comm, MPI_Errhandler errhandler) {
    return MPI_Errhandler_set(comm, errhandler);
}

int Exscan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
    return MPI_Exscan(sendbuf, recvbuf, count, datatype, op, comm);
}

int File_close(MPI_File *fh) {
    return MPI_File_close(fh);
}

int File_get_position(MPI_File fh, MPI_Offset *offset) {
    return MPI_File_get_position(fh, offset);
}

int File_open(MPI_Comm comm, const char *filename, int amode, MPI_Info info, MPI_File *fh) {
    return MPI_File_open(comm, filename, amode, info, fh);
}

int File_seek(MPI_File fh, MPI_Offset offset, int whence) {
    return MPI_File_seek(fh, offset, whence);
}

int File_set_size(MPI_File fh, MPI_Offset size) {
    return MPI_File_set_size(fh, size);
}

int File_write_at_all(MPI_File fh, MPI_Offset offset, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
    return MPI_File_write_at_all(fh, offset, buf, count, datatype, status);
}

int Finalize(void) {
    return MPI_Finalize();
}

int Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf,
           int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm) {
    return MPI_Gather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm);
}

int Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[],
            const int displs[], MPI_Datatype recvtype, int root, MPI_Comm comm) {
    return MPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm);
}

int Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf,
              int recvcount, MPI_Datatype recvtype, MPI_Comm comm) {
    return MPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
}

int Allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[],
               const int displs[], MPI_Datatype recvtype, MPI_Comm comm) {
    return MPI_Allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm);
}

int Init(int *argc, char ***argv) {
    return MPI_Init(argc, argv);
}

int Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request) {
    return MPI_Irecv(buf, count, datatype, source, tag, comm, request);
}

int Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
    return MPI_Isend(buf, count, datatype, dest, tag, comm, request);
}

int Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status) {
    return MPI_Recv(buf, count, datatype, source, tag, comm, status);
}

int Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm) {
    return MPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm);
}

int Type_commit(MPI_Datatype *type) {
    return MPI_Type_commit(type);
}

int Type_contiguous(int count, MPI_Datatype oldtype, MPI_Datatype *newtype) {
    return MPI_Type_contiguous(count, oldtype, newtype);
}

int Waitall(int count, MPI_Request array_of_requests[], MPI_Status *array_of_statuses) {
    return MPI_Waitall(count, array_of_requests, array_of_statuses);
}

int Probe(int source, int tag, MPI_Comm comm, MPI_Status *status) {
    return MPI_Probe(source, tag, comm, status);
}

int Get_count(const MPI_Status *status, MPI_Datatype datatype, int *count) {
    return MPI_Get_count(status, datatype, count);
}

int Error_string(int errorcode, char *string, int *resultlen) {
    return MPI_Error_string(errorcode, string, resultlen);
}

bool is_master(MPI_Comm comm) {
    int rank;
    Comm_rank(comm, &rank);
    return rank == 0;
}

int is_success(int errorcode) { return errorcode == MPI_SUCCESS; }
int is_pending(int errorcode) { return errorcode == MPI_ERR_PENDING; }
int is_err_in_status(int errorcode) { return errorcode == MPI_ERR_IN_STATUS; }

int status2errcode(MPI_Status *status) { return status->MPI_ERROR; }
}
