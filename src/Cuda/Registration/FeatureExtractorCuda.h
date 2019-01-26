//
// Created by wei on 1/23/19.
//

#include <Core/Geometry/PointCloud.h>
#include <Core/Geometry/KDTreeFlann.h>
#include <Cuda/Geometry/PointCloudCuda.h>
#include <Cuda/Registration/CorrespondenceSetCuda.h>

#pragma once

namespace open3d {
namespace cuda {

class FeatureCudaDevice {
public:
    PointCloudCudaDevice pcl_;

    CorrespondenceSetCudaDevice neighbors_;

    Array2DCudaDevice<float> spfh_features_;
    Array2DCudaDevice<float> fpfh_features_;

    __DEVICE__
    Vector4f ComputePairFeature(int i, int j);
    __DEVICE__
    void ComputeSPFHFeature(int i, int max_nn);
    __DEVICE__
    void ComputeFPFHFeature(int i, int max_nn);
};

class FeatureExtractorCuda {
public:
    std::shared_ptr<FeatureCudaDevice> server_ = nullptr;

public:
    FeatureExtractorCuda();
    ~FeatureExtractorCuda();
    FeatureExtractorCuda(const FeatureExtractorCuda &other);
    FeatureExtractorCuda& operator=(const FeatureExtractorCuda &other);

    void Create();
    void Release();
    void UpdateServer();

public:
    void Compute(PointCloud &pcl, const KDTreeSearchParamHybrid &param);

public:
    PointCloudCuda pcl_;

    CorrespondenceSetCuda neighbors_;
    Array2DCuda<float> spfh_features_;
    Array2DCuda<float> fpfh_features_;
};

class FeatureCudaKernelCaller {
public:
    static void ComputeSPFHFeature(FeatureExtractorCuda &feature);
    static void ComputeFPFHFeature(FeatureExtractorCuda &feature);
};

__GLOBAL__
void ComputeSPFHFeatureKernel(FeatureCudaDevice server);
__GLOBAL__
void ComputeFPFHFeatureKernel(FeatureCudaDevice server);

}
}

