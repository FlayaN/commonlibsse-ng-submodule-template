# SKSE NG Plugin template
Yet another Skyrim script extender plugin template using my preferred setup

## Initial setup

Set Author and Project Name here
https://github.com/FlayaN/commonlibsse-ng-submodule-template/blob/main/CMakeLists.txt#L3-L5

## Build

### Register Visual Studio as a Generator

- Open `x64 Native Tools Command Prompt`
- Run `cmake`
- Close the cmd window

```bat
rd /s /q "%~dp0/build"
cmake --preset build-release-msvc-msvc
cmake --build build --preset release-msvc-msvc
```

## Dependencies SSE

- [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
- [SKSE64](https://skse.silverlock.org/)
- [CLibUtil](https://github.com/powerof3/CLibUtil) (cmake portfile)

## Dependencies VR

- [VR Address Library for SKSEVR Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/58101)
- [SKSEVR](https://skse.silverlock.org/)
- [CLibUtil](https://github.com/powerof3/CLibUtil) (cmake portfile)
