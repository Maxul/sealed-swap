#!/bin/bash

set -ex

touch /tmp/backing
rm -rf /tmp/vram
mkdir -p /tmp/vram
bin/vramfs /tmp/vram 1G

path="/tmp/vram/swapfile"
truncate -s $((1000*1024*1024)) "$path"
chmod 600 "$path"

dev=$(losetup --find "$path" --show)
echo "$dev"
mkswap "$dev"
swapon "$dev"

free -h
