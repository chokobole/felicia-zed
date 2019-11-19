# Copyright (c) 2019 The Felicia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("//third_party:cuda_configure.bzl", "cuda_configure")
load("//third_party:zed_configure.bzl", "zed_configure")

def zed_deps():
    native.bind(
        name = "felicia",
        actual = "@com_github_chokobole_felicia//felicia:felicia",
    )

    native.bind(
        name = "zed",
        actual = "@local_config_zed//:zed",
    )

    native.bind(
        name = "cuda",
        actual = "@local_config_cuda//:cuda",
    )

    cuda_configure(name = "local_config_cuda")
    zed_configure(name = "local_config_zed")

    if not native.existing_rule("com_github_chokobole_felicia"):
        git_repository(
            name = "com_github_chokobole_felicia",
            remote = "https://github.com/chokobole/felicia.git",
            commit = "c07f4d588fe628c362805a3ba9e5688437661436",
        )

