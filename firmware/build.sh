#!/bin/bash
set -e

cmake -S . -B build -D PICO_BOARD=seeed_xiao_rp2040
make -C build -j $(getconf _NPROCESSORS_ONLN)
