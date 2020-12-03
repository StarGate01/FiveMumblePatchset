@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86

@REM mkdir protobuf 2> NUL
@REM cd protobuf
@REM cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_BUILD_TYPE=Release --config Release -Dprotobuf_BUILD_PROTOC_BINARIES=OFF -Dprotobuf_WITH_ZLIB=OFF ..\fivem\vendor\protobuf\cmake
@REM cd ..

@REM cd fivem\vendor\libuv
@REM git apply ..\..\..\patch\libuv.patch
@REM cd ..\..\..\
@REM mkdir libuv 2> NUL
@REM cd libuv
@REM cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_BUILD_TYPE=Release --config Release -DBUILD_TESTING=OFF ..\fivem\vendor\libuv
@REM cd ..

@REM cd fivem\vendor\labsound
@REM git apply ..\..\..\patch\labsound.patch
@REM cd ..\..\..\
@REM mkdir labsound 2> NUL
@REM cd labsound
@REM cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_BUILD_TYPE=Release CXXFLAGS --config Release ..\fivem\vendor\labsound
@REM cd ..

@REM cd fivem\vendor\fmtlib
@REM git apply ..\..\..\patch\fmtlib.patch
@REM cd ..\..\..\
@REM mkdir fmtlib 2> NUL
@REM cd fmtlib
@REM cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_BUILD_TYPE=Release --config Release ..\fivem\vendor\fmtlib
@REM cd ..

@REM cd botan\botan
@REM python .\configure.py --cc=msvc --os=windows --define-build-macro=_DISABLE_EXTENDED_ALIGNED_STORAGE --cpu=x86_32 --msvc-runtime=MT --disable-shared-library --prefix=.
@REM cd ..\..

cd ffmpeg
git apply ..\patch\ffmpeg.patch
"C:\msys64\msys2_shell.cmd" -defterm -mingw64 -no-start -full-path -here -use-full-path -c "./setup.sh"
cd ..