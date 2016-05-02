# cdh-utilities
This repository contains utilities that support different tasks around MOVE-II's CDH platform.

---

## create\_image.sh
This tool creates a complete Linux image from the yocto build artifacts (i.e. rootfs, kernel, kernel-modules, ...).

### Usage
```bash
./create_image.sh deploy_dir [image_name]
```

### Options
* *deploy\_dir*

    The path to the yocto image deploy directory, in which the build artifacts are located.

* *image\_name*

    Name of the image that you built. Defaults to _core-image-cdh_.

