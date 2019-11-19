# Copyright (c) 2019 The Felicia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

load("@com_github_chokobole_felicia//bazel:felicia_cc.bzl", "fel_cc_native_library")

fel_cc_native_library(
    name = "zed",
    srcs = [
        "zed/zed_camera.cc",
        "zed/zed_camera_descriptor.cc",
        "zed/zed_camera_factory.cc",
        "zed/zed_capability.cc",
    ],
    hdrs = [
        "zed/zed_camera.h",
        "zed/zed_camera_descriptor.h",
        "zed/zed_camera_factory.h",
        "zed/zed_capability.h",
    ],
    deps = [
        "//external:felicia",
        "//external:zed",
        "//external:cuda",
    ],
    visibility = ["//visibility:public"],
)
