# Felicia Zed

## Prerequisites

Follow the instruction at [Installation Guide](https://www.stereolabs.com/docs/getting-started/installation/).

## WORKSPACE setup

```python
bind(
    name = "felicia",
    actual = "@com_github_chokobole_felicia//felicia:felicia",
)

bind(
    name = "felicia_zed",
    actual = "@com_github_chokobole_felicia_zed//:zed",
)

git_repository(
    name = "com_github_chokobole_felicia",
    remote = "https://github.com/chokobole/felicia.git",
    commit = "<commit>",
)

git_repository(
    name = "com_github_chokobole_felicia_zed",
    remote = "https://github.com/chokobole/felicia-zed.git",
    commit = "<commit>",
)

load("@com_github_chokobole_felicia//bazel:felicia_deps.bzl", "felicia_deps")

felicia_deps()

load("@com_github_chokobole_felicia_zed//:zed_deps.bzl", "zed_deps")

zed_deps()
```

## Example

```python
load("@com_github_chokobole_felicia//bazel:felicia_cc.bzl", "fel_cc_native_binary")

fel_cc_native_binary(
    name = "<name>",
    ...,
    deps = [
        "//external:felicia_zed",
    ],
)
```

Check out examples [here](https://github.com/chokobole/felicia-examples/blob/master/examples/learn/topic/stereo_camera/cc/BUILD).