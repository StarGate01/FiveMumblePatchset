@echo off

mkdir protobuf 2> NUL
cd protobuf
cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_PROTOC_BINARIES=OFF -Dprotobuf_WITH_ZLIB=OFF ..\fivem\vendor\protobuf\cmake
cd ..

mkdir libuv 2> NUL
cd libuv
cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF ..\fivem\vendor\libuv
cd ..

cd botan\botan
python .\configure.py --cc=msvc --os=windows --define-build-macro=_DISABLE_EXTENDED_ALIGNED_STORAGE --cpu=x86_32 --prefix=..\Release
cd ..\..