#!/bin/bash

set -ex

rm /tmp/backing
swapoff -a
losetup -d /dev/loop0
fusermount -u /tmp/vram && echo "unmounted VRAM file system"
