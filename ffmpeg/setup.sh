#!/bin/bash

source config.sh
cd ffmpeg

echo "Configuring FFmpeg"

./configure --toolchain=msvc --prefix=../Release --disable-debug --arch=x86_32 --enable-static --disable-shared --enable-gpl --disable-autodetect 