namespace cnt {
int nsolutes;
DeviceBuffer<uchar4> *subindices;
DeviceBuffer<unsigned char> *compressed_cellscount;
DeviceBuffer<int> *cellsentries, *cellsstart, *cellscount;
rnd::KISS* local_trunk;
}
