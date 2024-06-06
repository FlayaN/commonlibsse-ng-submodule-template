# CommonLibSSE NG Plugin template
Yet another Skyrim script extender plugin template using my preferred setup

## Initial setup

Set Author and Project Name here
https://github.com/FlayaN/commonlibsse-ng-submodule-template/blob/main/CMakeLists.txt#L3-L5

## Requirements

- [CMake](https://cmake.org/)
  - Add this to your `PATH`
- [Vcpkg](https://github.com/microsoft/vcpkg)
  - Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
- [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
  - Desktop development with C++

## User Requirements

- [Address Library for SKSE](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
  - Needed for SSE/AE
- [VR Address Library for SKSEVR](https://www.nexusmods.com/skyrimspecialedition/mods/58101)
  - Needed for VR

## Register Visual Studio as a Generator

- Open `x64 Native Tools Command Prompt`
- Run `cmake`
- Close the cmd window

## Building

```
# to update submodules in /extern
git submodule update --init --recursive
# configure cmake
cmake --preset build-release-msvc-msvc
# build dll
cmake --build build --preset release-msvc-msvc
```
