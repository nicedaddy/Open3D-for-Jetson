//
// Created by wei on 11/5/18.
//

#include "RGBDImageCuda.h"

namespace open3d {
RGBDImageCuda::RGBDImageCuda(float depth_near,
                             float depth_far,
                             float depth_factor)
    : depth_near_(depth_near),
      depth_far_(depth_far),
      depth_factor_(depth_factor) {}

RGBDImageCuda::RGBDImageCuda(int width, int height,
                             float depth_near,
                             float depth_far,
                             float depth_factor)
    : depth_near_(depth_near),
      depth_far_(depth_far),
      depth_factor_(depth_factor) {
    Create(width, height);
}

RGBDImageCuda::RGBDImageCuda(const open3d::RGBDImageCuda &other) {
    server_ = other.server();

    depth_near_ = other.depth_near_;
    depth_far_ = other.depth_far_;
    depth_factor_ = other.depth_factor_;

    depth_raw_ = other.depth_raw();
    depthf_ = other.depthf();
    color_ = other.color();
    intensity_ = other.intensity();
}


RGBDImageCuda& RGBDImageCuda::operator=(const open3d::RGBDImageCuda &other) {
    if (this != &other) {
        Release();

        server_ = other.server();

        depth_near_ = other.depth_near_;
        depth_far_ = other.depth_far_;
        depth_factor_ = other.depth_factor_;

        depth_raw_ = other.depth_raw();
        depthf_ = other.depthf();
        color_ = other.color();
        intensity_ = other.intensity();
    }
    return *this;
}

RGBDImageCuda::~RGBDImageCuda() {
    Release();
}

void RGBDImageCuda::Create(int width, int height) {
    assert(width > 0 && height > 0);

    if (server_ != nullptr) {
        PrintError("[RGBDImageCuda] Already created, abort!\n");
        return;
    }

    server_ = std::make_shared<RGBDImageCudaServer>();

    depth_raw_.Create(width, height);
    color_.Create(width, height);

    depthf_.Create(width, height);
    intensity_.Create(width, height);

    UpdateServer();
}

void RGBDImageCuda::Release() {
    server_ = nullptr;

    depth_raw_.Release();
    color_.Release();

    depthf_.Release();
    intensity_.Release();
}

void RGBDImageCuda::Upload(cv::Mat &depth, cv::Mat &color) {
    if (server_ == nullptr) {
        server_ = std::make_shared<RGBDImageCudaServer>();
    }

    depth_raw_.Upload(depth);
    color_.Upload(color);

    depth_raw_.ConvertToFloat(depthf_, 1.0f / depth_factor_);
    color_.ConvertRGBToIntensity(intensity_);

    UpdateServer();
}

void RGBDImageCuda::Upload(Image &depth_raw, Image &color_raw) {
    if (server_ == nullptr) {
        server_ = std::make_shared<RGBDImageCudaServer>();
    }

    depth_raw_.Upload(depth_raw);
    color_.Upload(color_raw);

    depth_raw_.ConvertToFloat(depthf_, 1.0f / depth_factor_);
    color_.ConvertRGBToIntensity(intensity_);

    UpdateServer();
}

void RGBDImageCuda::CopyFrom(
    ImageCuda<Vector1s> &depth_raw, ImageCuda<Vector3b> &color_raw) {
    if (server_ == nullptr) {
        server_ = std::make_shared<RGBDImageCudaServer>();
    }

    depth_raw_.CopyFrom(depth_raw);
    color_.CopyFrom(color_raw);
    depth_raw_.ConvertToFloat(depthf_, 1.0f / depth_factor_);
    color_.ConvertRGBToIntensity(intensity_);

    UpdateServer();
}

void RGBDImageCuda::UpdateServer() {
    if (server_ != nullptr) {
        server_->depth() = *depthf_.server();
        server_->color() = *color_.server();
        server_->intensity() = *intensity_.server();
    }
}
}