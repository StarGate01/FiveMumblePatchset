# FiveMumblePatchset

A set up tools and patches to compile the Mumble-Client component of the FiveM project (https://github.com/citizenfx/fivem) to a standalone Win32 DLL.

## Building

Tested on Windows 10 x64.

Pull this repository using `git clone --recurse-submodules` to download all dependencies (a few GB).

Make sure your System contains:
 - Visual Studio Community 2019
 - CMake >= 3.15
 - Msys2 (x64)
   - make
   - autoconf
   - diffutils
 - Yasm
 - Python
 - Git

If needed, adjust the toolchain paths in `setup.bat` and `ffmpeg/build.sh`.

**Run `setup.bat`**.  This will apply a few patches to the build toolchains and generate the missing Visual Studio projects by invoking CMake or other build tools. **Then** open the Visual Studio Solution. Compile the `libfivemumble` or `testapp` project.

## Subprojects and how they are generated

- ffmpeg (autotools) (Requires Msys2)
- botan (python)
- enet (VS)
- fmt (CMake)
- tbb (VS)
- uv (CMake)
- webrtc (VS)
- libprotobuf (CMake)
- LabSound (CMake)
  - libnyquist
  - libopus
  - libwavpack
- libfivemumble (VS)
- testapp (VS)

## Static linking

- ws2_32.lib
- winmm.lib
- libfivemumble.lib
- botan.lib
- enet.lib
- tbb.lib (Requires /SAFESEH:NO)
- uv_a.lib
- libprotobuf.lib
- LabSound.lib
- libnyquist.lib
- libopus.lib
- libwavpack.lib
- fmt.lib
- webrtc.lib
- user32.lib
- avcodec.lib
- avdevice.lib
- avfilter.lib
- avformat.lib
- avutil.lib
- postproc.lib
- swresample.lib
- swscale.lib
- bcrypt.lib
- Iphlpapi.lib
- Advapi32.lib
- Kernel32.lib
- Psapi.lib
- Userenv.lib