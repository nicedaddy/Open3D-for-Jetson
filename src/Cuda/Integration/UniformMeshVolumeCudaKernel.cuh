//
// Created by wei on 10/20/18.
//

#include "UniformMeshVolumeCuda.cuh"

namespace open3d {
template<VertexType type, size_t N>
__global__
void MarchingCubesVertexAllocationKernel(
    UniformMeshVolumeCudaServer<type, N> server,
    UniformTSDFVolumeCudaServer<N> tsdf_volume) {
    const int x = threadIdx.x + blockIdx.x * blockDim.x;
    const int y = threadIdx.y + blockIdx.y * blockDim.y;
    const int z = threadIdx.z + blockIdx.z * blockDim.z;

    if (x >= N - 1 || y >= N - 1 || z >= N - 1) return;

    Vector3i Xlocal = Vector3i(x, y, z);
    server.AllocateVertex(Xlocal, tsdf_volume);
}

template<VertexType type, size_t N>
__global__
void MarchingCubesVertexExtractionKernel(
    UniformMeshVolumeCudaServer<type, N> server,
    UniformTSDFVolumeCudaServer<N> tsdf_volume) {
    const int x = threadIdx.x + blockIdx.x * blockDim.x;
    const int y = threadIdx.y + blockIdx.y * blockDim.y;
    const int z = threadIdx.z + blockIdx.z * blockDim.z;

    Vector3i Xlocal = Vector3i(x, y, z);
    server.ExtractVertex(Xlocal, tsdf_volume);
}

template<VertexType type, size_t N>
__global__
void MarchingCubesTriangleExtractionKernel(
    UniformMeshVolumeCudaServer<type, N> server) {
    const int x = threadIdx.x + blockIdx.x * blockDim.x;
    const int y = threadIdx.y + blockIdx.y * blockDim.y;
    const int z = threadIdx.z + blockIdx.z * blockDim.z;

    if (x >= N - 1 || y >= N - 1 || z >= N - 1) return;

    Vector3i Xlocal = Vector3i(x, y, z);
    server.ExtractTriangle(Xlocal);
}
}