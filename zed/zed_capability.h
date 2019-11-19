// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ZED_ZED_CAPABILITY_H_
#define ZED_ZED_CAPABILITY_H_

#include <sl/defines.hpp>

#include "felicia/drivers/camera/camera_format.h"

namespace felicia {
namespace drivers {

struct ZedCapability {
  sl::RESOLUTION resolution;
  float frame_rate;
};

const ZedCapability kZedCapabilities[] = {
    {sl::RESOLUTION_HD2K, 15},   {sl::RESOLUTION_HD1080, 15},
    {sl::RESOLUTION_HD1080, 30}, {sl::RESOLUTION_HD720, 15},
    {sl::RESOLUTION_HD720, 30},  {sl::RESOLUTION_HD720, 60},
    {sl::RESOLUTION_VGA, 15},    {sl::RESOLUTION_VGA, 30},
    {sl::RESOLUTION_VGA, 60},    {sl::RESOLUTION_VGA, 100},
};

CameraFormat ConvertToCameraFormat(const ZedCapability& capability);

const ZedCapability* GetBestMatchedCapability(const CameraFormat& requested);

}  // namespace drivers
}  // namespace felicia

#endif  // ZED_ZED_CAPABILITY_H_