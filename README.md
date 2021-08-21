# Engine

An early WIP version of my personal engine using C++20.

The engine will likely switch over to C++20 modules once support in tooling has improved.

# Build

## Windows

### VS2019

Requirements
- VS2019
- premake5 (in tools/premake/)
- CMake (added to Path, for third party libs)
- msbuild (added to Path, for third party libs)

Setup
- run download_third-party.bat
- run premake.bat
- open and build VS2019 project