// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ZED_CAMERA_H_
#define ZED_CAMERA_H_

#include <sl/Camera.hpp>

#include "chromium/base/memory/weak_ptr.h"
#include "chromium/base/synchronization/waitable_event.h"
#include "chromium/base/threading/thread.h"
#include "felicia/core/lib/coordinate/coordinate.h"
#include "felicia/drivers/camera/depth_camera_frame.h"
#include "felicia/drivers/camera/stereo_camera_interface.h"
#include "felicia/map/pointcloud.h"

#include "zed/zed_camera_descriptor.h"
#include "zed/zed_capability.h"

namespace felicia {
namespace drivers {

class ZedCamera : public StereoCameraInterface,
                  public base::SupportsWeakPtr<ZedCamera> {
 public:
  class ScopedCamera {
   public:
    ScopedCamera();
    ScopedCamera(ScopedCamera&& other);
    void operator=(ScopedCamera&& other);

    ~ScopedCamera();

    const sl::Camera* operator->() const { return get(); }
    sl::Camera* operator->() { return get(); }

    const sl::Camera* get() const { return camera_.get(); }
    sl::Camera* get() { return camera_.get(); }

   private:
    std::unique_ptr<sl::Camera> camera_;

    DISALLOW_COPY_AND_ASSIGN(ScopedCamera);
  };

  // |requested_color_format| will overwrite |params|'s |camera_fps|
  // and |camera_resolution|.
  struct StartParams {
    CameraFormat requested_camera_format;
    sl::InitParameters init_params;
    sl::RuntimeParameters runtime_params;
    base::TimeDelta pointcloud_interval;

    CameraFrameCallback left_camera_frame_callback;
    CameraFrameCallback right_camera_frame_callback;
    DepthCameraFrameCallback depth_camera_frame_callback;
    map::PointcloudCallback pointcloud_callback;
    StatusCallback status_callback;
  };

  ~ZedCamera();

  static bool IsSameId(const std::string& device_id,
                       const std::string& device_id2);

  // StereoCameraInterface methods
  Status Init() override;
  Status Start(const CameraFormat& requested_camera_format,
               CameraFrameCallback left_camera_frame_callback,
               CameraFrameCallback right_camera_frame_callback,
               DepthCameraFrameCallback depth_camera_frame_callback,
               StatusCallback status_callback) override;
  Status Stop() override;

  Status SetCameraSettings(const CameraSettings& camera_settings) override;
  Status GetCameraSettingsInfo(
      CameraSettingsInfoMessage* camera_settings) override;

  Status Start(const StartParams& params);

  sl::Camera* camera() { return camera_.get(); }

  const sl::Camera* camera() const { return camera_.get(); }

 private:
  friend class ZedCameraFactory;

  ZedCamera(const ZedCameraDescriptor& camera_descriptor);

  void GetCameraSetting(sl::CAMERA_SETTINGS camera_setting,
                        CameraSettingsModeValue* value);
  void GetCameraSetting(sl::CAMERA_SETTINGS camera_setting,
                        CameraSettingsRangedValue* value);

  void DoGrab();
  void DoStop(base::WaitableEvent* event, Status* s);

  CameraFrame ConvertToCameraFrame(sl::Mat image, base::TimeDelta timestamp);

  DepthCameraFrame ConvertToDepthCameraFrame(sl::Mat image,
                                             base::TimeDelta timestamp,
                                             float min, float max);

  map::Pointcloud ConvertToPointcloud(sl::Mat cloud, base::TimeDelta timestamp);

  static Status OpenCamera(const int device_id, sl::InitParameters& params,
                           ScopedCamera* camera);

  int device_id_ = -1;
  ScopedCamera camera_;
  sl::InitParameters init_params_;
  sl::RuntimeParameters runtime_params_;

  DepthCameraFrameCallback depth_camera_frame_callback_;
  map::PointcloudCallback pointcloud_callback_;

  base::Thread thread_;
  base::Lock lock_;
  bool is_stopping_ GUARDED_BY(lock_);

  base::TimeDelta last_pointcloud_timestamp_;
  base::TimeDelta pointcloud_interval_;

  Coordinate coordinate_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ZedCamera);
};

}  // namespace drivers
}  // namespace felicia

#endif  // ZED_CAMERA_H_