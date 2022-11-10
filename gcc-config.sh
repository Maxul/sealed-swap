set -ex

scl enable devtoolset-8 -- bash
export PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig/:$PKG_CONFIG_PATH
