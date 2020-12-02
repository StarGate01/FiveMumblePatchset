@echo off

@REM mkdir protobuf 2> NUL
@REM cd protobuf
@REM cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_BUILD_TYPE=Release --config Release -Dprotobuf_BUILD_PROTOC_BINARIES=OFF -Dprotobuf_WITH_ZLIB=OFF ..\fivem\vendor\protobuf\cmake
@REM cd ..

@REM mkdir libuv 2> NUL
@REM cd libuv
@REM cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_BUILD_TYPE=Release --config Release -DBUILD_TESTING=OFF ..\fivem\vendor\libuv
@REM cd ..

cd fivem\vendor\labsound
git apply ..\..\..\patch\labsound.patch
cd ..\..\..\
mkdir labsound 2> NUL
cd labsound
cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_BUILD_TYPE=Release CXXFLAGS --config Release ..\fivem\vendor\labsound
cd ..

@REM cd botan\botan
@REM python .\configure.py --cc=msvc --os=windows --define-build-macro=_DISABLE_EXTENDED_ALIGNED_STORAGE --cpu=x86_32 --msvc-runtime=MT --prefix=..\Release
@REM cd ..\..