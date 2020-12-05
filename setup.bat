@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86

mkdir protobuf 2> NUL
cd protobuf
cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_BUILD_TYPE=Release --config Release -Dprotobuf_BUILD_PROTOC_BINARIES=OFF -Dprotobuf_WITH_ZLIB=OFF ..\fivem\vendor\protobuf\cmake
cd ..

cd fivem\vendor\libuv
git apply ..\..\..\patch\libuv.patch
cd ..\..\..\
mkdir libuv 2> NUL
cd libuv
cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_BUILD_TYPE=Release --config Release -DBUILD_TESTING=OFF ..\fivem\vendor\libuv
cd ..

cd fivem\vendor\labsound
git apply ..\..\..\patch\labsound.patch
cd ..\..\..\
mkdir labsound 2> NUL
cd labsound
cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_BUILD_TYPE=Release CXXFLAGS --config Release ..\fivem\vendor\labsound
cd ..

cd fivem\vendor\fmtlib
git apply ..\..\..\patch\fmtlib.patch
cd ..\..\..\
mkdir fmtlib 2> NUL
cd fmtlib
cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_BUILD_TYPE=Release --config Release ..\fivem\vendor\fmtlib
cd ..

cd botan\botan
python .\configure.py --cc=msvc --os=windows --define-build-macro=_DISABLE_EXTENDED_ALIGNED_STORAGE --cpu=x86_32 --msvc-runtime=MT --disable-shared-library --prefix=.
cd ..\..

cd ffmpeg
git apply ..\patch\ffmpeg.patch
"C:\msys64\msys2_shell.cmd" -defterm -mingw64 -no-start -full-path -here -use-full-path -c "./setup.sh"
cd ..