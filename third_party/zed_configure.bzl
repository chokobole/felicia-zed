# Copyright (c) 2019 The Felicia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

_ZED_PATH = "ZED_PATH"
_DEFAULT_PATH_ON_LINUX = "/usr/local/zed"
_DEFAULT_PATH_ON_WINDOWS = "C:\\Program Files (x86)\\ZED SDK"

def _is_linux(repository_ctx):
    """Returns true if the host operating system is linux."""
    os_name = repository_ctx.os.name.lower()
    return os_name.find("linux") != -1

def _is_windows(repository_ctx):
    """Returns true if the host operating system is windows."""
    os_name = repository_ctx.os.name.lower()
    return os_name.find("windows") != -1

def _symlink_dir(repository_ctx, src_dir, dest_dir):
    files = repository_ctx.path(src_dir).readdir()
    for src_file in files:
        repository_ctx.symlink(src_file, dest_dir + "/" + src_file.basename)

def _fail(msg):
    """Output failure message when auto configuration fails."""
    fail("Zed Confiugation Error: %s\n" % msg)

def _get_zed_path(repository_ctx):
    zed_path = repository_ctx.os.environ.get(_ZED_PATH)
    if zed_path == None:
        if _is_linux(repository_ctx):
            zed_path = _DEFAULT_PATH_ON_LINUX
        elif _is_windows(repository_ctx):
            zed_path = _DEFAULT_PATH_ON_WINDOWS

    if repository_ctx.path(zed_path).exists:
        return zed_path
    else:
        _fail("Failed to find the path for zed, Did you installed sdk " +
              "by default? or please locate the sdk path setting " +
              "enviornment variable \"ZED_PATH\"")

def _zed_configure_impl(repository_ctx):
    repository_ctx.symlink(Label("//third_party:zed.BUILD"), "BUILD")

    path = _get_zed_path(repository_ctx)
    zed_include = path + "/include"
    zed_lib = path + "/lib"
    _symlink_dir(repository_ctx, zed_include, "zed_include")
    _symlink_dir(repository_ctx, zed_lib, "zed_lib")

    if _is_windows(repository_ctx):
        zed_bin = path + "/bin"
        _symlink_dir(repository_ctx, zed_bin, "zed_bin")

zed_configure = repository_rule(
    implementation = _zed_configure_impl,
    environ = [
        _ZED_PATH,
    ],
)
"""Detects and configure the zed header and library

Add the following to your WORKSPACE FILE:

```python
zed_configure(name = "local_config_zed")
```
Args:
    name: A unique name for the workspace rule.
"""
