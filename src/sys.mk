LINK = g++
NVCC_DIR = /usr/local/cuda-8.0
NVCC     = ${NVCC_DIR}/bin/nvcc

MPI_CXXFLAGS = -I/usr/include/mpich-x86_64
MPI_LIBS  = -L/usr/lib64/mpich/lib -Wl,-rpath -Wl,/usr/lib64/mpich/lib -lmpichcxx -lmpich

HDF5_DIR=${HOME}/libs/hdf5
HDF5_CXXFLAGS = -I${HDF5_DIR}/include
HDF5_LIBS     = -L${HDF5_DIR}/lib -Wl,-rpath -Wl,${HDF5_DIR}/lib -lhdf5
NVCC_LIBS = -L${NVCC_DIR}/lib64 -Wl,-rpath -Wl,${NVCC_DIR}/lib64

CXXFLAGS += -Wall -Wextra -Wno-unused-parameter -Wno-unused-function
