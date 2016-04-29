# cdh-utilities
This repository contains utilities that support different tasks around MOVE-II's CDH platform.

---

## create\_image.sh
This tool creates a complete Linux image from the yocto build artifacts (i.e. rootfs, kernel, kernel-modules, ...).

#### Usage:
```bash
./create_image.sh deploy_dir
```
Note: _deploy\_dir_ is the path to the yocto image deploy directory, in which the build artifacts are located.

