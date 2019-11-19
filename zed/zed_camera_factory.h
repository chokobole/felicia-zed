// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ZED_ZED_CAMERA_FACTORY_H_
#define ZED_ZED_CAMERA_FACTORY_H_

#include "chromium/base/macros.h"

#include "zed/zed_camera.h"
#include "zed/zed_camera_descriptor.h"

namespace felicia {
namespace drivers {

class ZedCameraFactory {
 public:
  static std::unique_ptr<ZedCamera> NewStereoCamera(
      const ZedCameraDescriptor& camera_descriptor);

  static Status GetCameraDescriptors(ZedCameraDescriptors* camera_descriptors);

  static Status GetSupportedCameraFormats(
      const ZedCameraDescriptor& camera_descriptor,
      CameraFormats* camera_formats);

  DISALLOW_COPY_AND_ASSIGN(ZedCameraFactory);
};

}  // namespace drivers
}  // namespace felicia

#endif  // ZED_ZED_CAMERA_FACTORY_H_