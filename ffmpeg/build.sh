#!/bin/bash

echo "Building FFmpeg"
cd ffmpeg

export PATH="/c/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29333/bin/Hostx64/x86":$PATH

echo "Running in: `pwd`"
echo "Link: `which link`"

./configure --toolchain=msvc --prefix=../Release --disable-debug --arch=x86_32 --enable-static --disable-shared --enable-gpl --disable-autodetect 
make -j12
make install
