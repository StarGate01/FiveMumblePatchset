#!/bin/bash

source config.sh
cd ffmpeg

echo "Building FFmpeg"

make -j$(nproc)
make install
