# Engine

An early WIP version of my personal engine using C++20.

The engine will likely switch over to C++20 modules once support in tooling has improved.

# Build

## Windows

### VS2019

Requirements
- VS2019
- premake5 (in tools/premake/)
- CMake (added to Path, for third party libs) (will be replaced with custom premake scripts in the future)
- msbuild (added to Path, for third party libs)

Setup
- run download_third-party.bat
- run premake.bat
- open and build VS2019 project
- currently there is an issue with premake not setting the configurations for C# correctly, 
  so when opening the project, open the configuration manager and make sure to go through 
  all configuration to let vs updated them to the correct option