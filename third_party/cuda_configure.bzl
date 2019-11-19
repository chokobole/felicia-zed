# Copyright (c) 2019 The Felicia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

_CUDA_PATH = "CUDA_PATH"

def _get_bin_path(repository_ctx, bin, env_var = None):
    bin_path = None
    if env_var != None:
        bin_path = repository_ctx.os.environ.get(env_var)
    if bin_path == None:
        bin_path = repository_ctx.which(bin)
    return bin_path

def _get_bash_bin_path(repository_ctx):
    """Gets the bash bin path."""
    return _get_bin_path(repository_ctx, "bash", "BAZEL_SH")

def _is_executable(repository_ctx, bin, env_var = None):
    """Check bin whether exists and is executable."""
    bin_path = _get_bin_path(repository_ctx, bin, env_var)
    if bin_path == None:
        return False
    cmd = "test -x \"%s\"" % bin_path
    bash_bin = _get_bash_bin_path(repository_ctx)
    if bash_bin == None:
        return False
    result = repository_ctx.execute([bash_bin, "-c", cmd])
    return result.return_code == 0

def _symlink_dir(repository_ctx, src_dir, dest_dir):
    files = repository_ctx.path(src_dir).readdir()
    for src_file in files:
        repository_ctx.symlink(src_file, dest_dir + "/" + src_file.basename)

def _fail(msg):
    """Output failure message when auto configuration fails."""
    fail("Cuda Confiugation Error: %s\n" % msg)

def _cuda_configure_impl(repository_ctx):
    repository_ctx.symlink(Label("//third_party:cuda.BUILD"), "BUILD")

    if not _is_executable(repository_ctx, "nvcc"):
        _fail("nvcc doesn't exist or is not executable.")
    nvcc_bin_path = _get_bin_path(repository_ctx, "nvcc")
    cmd = [nvcc_bin_path, "-v", "/dev/null", "-o", "/dev/null"]
    result = repository_ctx.execute(cmd)

    target_size = None
    top = None
    TARGET_SIZE_PREFIX = "#$ _TARGET_SIZE_="
    TOP_PREFIX = "#$ TOP="

    # On windows prints to stdout, on linux prints to stderr, though.
    result_txt = result.stderr if len(result.stderr) > 0 else result.stdout
    for line in result_txt.splitlines():
        if line.startswith(TARGET_SIZE_PREFIX):
            target_size_str = line[len(TARGET_SIZE_PREFIX):]
            if len(target_size_str) == 0:
                continue
            target_size = int(target_size_str)
        if line.startswith(TOP_PREFIX):
            top = line[len(TOP_PREFIX):]

    if top == None:
        _fail("Failed to configure top")

    if target_size == None:
        _fail("Failed to configure target_size")

    cuda_include = top + "/include"
    _symlink_dir(repository_ctx, cuda_include, "cuda_include")

cuda_configure = repository_rule(
    implementation = _cuda_configure_impl,
    environ = [
        _CUDA_PATH,
    ],
)
"""Detects and configure the cuda header and library

Add the following to your WORKSPACE FILE:

```python
cuda_configure(name = "local_config_cuda")
```
Args:
    name: A unique name for the workspace rule.
"""
