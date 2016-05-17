#!/bin/bash
#
# create_img.sh - Script to create a flashable Linux Image for MOVE-II's CDH Devboard
#
# Copyright (C) 2016  Jonas Sticha (jonas.sticha@tum.de)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Stop script on error or if an unset variable is accessed
set -eu

# Define global variables
IMAGE_FILE=
ROOTFS_SIZE_MB="128"
BOOT_SIZE_MB="16"
IMAGE_SIZE=$((1+$BOOT_SIZE_MB+$ROOTFS_SIZE_MB))
IMAGE_NAME="core-image-cdh"
BOOT_BIN_URL="https://github.com/move-II/cdh-utilities/raw/binaries/BOOT.BIN"
UBOOT_BIN_URL="https://github.com/move-II/cdh-utilities/raw/binaries/u-boot.bin"
UBOOT_ENV_URL="https://github.com/move-II/cdh-utilities/raw/binaries/uboot.env"
UNCOMPLETE="true"
DEPLOY_DIR=
TMP_DIR=
LOOP_DEV=

function print_usage {
    echo "Usage: $0 deploy_path [image_name]"
}

function print_dashline {
    echo "-----------------------------------------------"
}

function allocate_image_file {
    print_dashline
    echo "Image size: ${IMAGE_SIZE}MB"
    print_dashline
    fallocate -l ${IMAGE_SIZE}M $IMAGE_FILE
}

function create_partitions {
    print_dashline
    echo "Creating partitions..."
    parted -s $IMAGE_FILE -- mktable msdos \
        mkpart p fat32 1 $(($BOOT_SIZE_MB)) \
        mkpart p ext4 $(($BOOT_SIZE_MB+1)) -0
    echo "Done:"
    parted $IMAGE_FILE print
    print_dashline
}

function setup_loopdevice {
    print_dashline
    echo "Setting up loop device for image..."
    LOOP_DEV=$(losetup -Pf --show $IMAGE_FILE)
    echo "Loop device set up on $LOOP_DEV."
    print_dashline
}

function detach_loopdevice {
    echo "Detaching loopdevice $LOOP_DEV"
    losetup -D $IMAGE_FILE
    LOOP_DEV=
}

function create_filesystems {
    print_dashline
    echo "Creating filesystems for partitions..."
    mkfs.vfat ${LOOP_DEV}p1
    mkfs.ext4 ${LOOP_DEV}p2
    echo "Done"
    print_dashline
}

function mount_tmpdir {
    TMP_DIR=$(mktemp -d)
    mount $1 $TMP_DIR
}

function umount_tmpdir {
    umount $TMP_DIR
    rm -r $TMP_DIR
    TMP_DIR=
}

function populate_boot_partition {
    print_dashline
    echo "Populating boot partition..."
    mount_tmpdir ${LOOP_DEV}p1
    echo "Boot partition mounted to $TMP_DIR"
    wget $BOOT_BIN_URL -O $TMP_DIR/BOOT.BIN
    wget $UBOOT_BIN_URL -O $TMP_DIR/u-boot.bin
    wget $UBOOT_ENV_URL -O $TMP_DIR/uboot.env
    cp $DEPLOY_DIR/zImage $TMP_DIR/zImage
    cp $DEPLOY_DIR/zImage-at91-sama5d2_xplained.dtb $TMP_DIR/at91-sama5d2_xplained.dtb
    umount_tmpdir
    echo
    echo "Boot partition completed successfully."
    print_dashline
}

function populate_rootfs {
    print_dashline
    echo "Populating rootfs..."
    mount_tmpdir ${LOOP_DEV}p2
    echo "Rootfs partition mounted to $TMP_DIR"
    tar xfz $DEPLOY_DIR/${IMAGE_NAME}-sama5d2-xplained.tar.gz -C $TMP_DIR
    tar xfz $DEPLOY_DIR/modules-sama5d2-xplained.tgz -C $TMP_DIR
    ls -lah $TMP_DIR
    umount_tmpdir
    echo
    echo "Rootfs populated successfully"
    print_dashline
}

function cleanup {
    print_dashline
    echo "Cleaning up..."
    if [ "$TMP_DIR" != "" ]
    then
        echo "Remove remaining temporary directory $TMP_DIR"
        umount_tmpdir
    fi
    if [ "$LOOP_DEV" != "" ]
    then
        detach_loopdevice
    fi
    if [ "$UNCOMPLETE" != "false" ]
    then
        rm -f $IMAGE_FILE
        echo "Remove unfinished image file $IMAGE_FILE"
    fi
    echo "Cleanup complete."
    print_dashline
}

######################
# Start of Execution #
######################

trap cleanup 0

# Check args
if [ "$#" -lt 1 ] || [ "$#" -gt 2 ]
then
    echo "ERROR: Wrong number of arguments."
    print_usage
    exit 1
fi
DEPLOY_DIR=$1

if [ "$#" == "2" ]
then
    IMAGE_NAME=$2
fi
IMAGE_FILE="Linux_${IMAGE_NAME}.img"

# Check for root permissions
if [ "$(whoami)" != "root" ]
then
    echo "ERROR: root permissions required. Try again with sudo."
    exit 1
fi

# Execute main process
allocate_image_file
create_partitions
setup_loopdevice
create_filesystems
populate_boot_partition
populate_rootfs
detach_loopdevice

print_dashline
echo "Image $IMAGE_FILE was created successfully."
UNCOMPLETE="false"
print_dashline
