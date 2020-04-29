# Citadel
Citadel is a cross-platform game engine, for 2D and 3D games. It is compiled into a single static library for each target.

## Targets
Each target is a combination, when possible, of the platform and graphics api.
- Windows supports DirectX 12 and Vulkan
- Linux will use Vulkan
- MacOS and iOS will use Metal
- Android will use OpenGL ES
- Web will use Web GL

All platform and apis will be abstracted so, for the most part, the same game code can be compiled for all targets.
Some advanced features may only be available on some targets without proper fallback for now. For example, raytracing may only be available on the Windows DirectX 12 target.

### Short names
Each platform will be identified via a 3 character short code
- Windows => win
- Linux => nix
- MacOS => mac
- iOS => ios
- Android => ???

Likewise, each graphics api will be identified via a short name
- DirectX 12 => dx12
- Vulkan => vlkn
- Metal => metl
- OpenGL ES => gles
- Web GL => wgl

These short names should be used for naming files that target specific graphics api or platforms, as well as naming folders to organize those files.
For example, the agnostic portion of the Graphics Device is found in Device.cpp, while the DX12 specific implementation is in Device.dx12.cpp
Similarly the Application.cpp class is completely agnostic, while the Application.win.cpp contains the platform specific portions of its implementation.

# Modules
Citadel is broken across multiple modules, listed here alphabetically, each providing either target abstraction, or implementation agnostic features.

## Abacus
Abacus is a math library. This library should be target agnostic.

## Ballista
Ballista is a physics library. This library should be target agnostic.

## Castle
Castle is the main platform abstraction, it should remain agnostic to the graphics api, but will be very platform specific in its implementations.

## Keep
Keep is a library full of very generic and basic types and utilities. It should primarily be target agnostic, but that is not a requirement.

## Sage
Sage is a library abstracting scripting languages for the engine. This library should strive to be platform agnostic as much as possible, but may require some platform dependence for various language implementations.
Languages planned for support are Lua and JavaScript.

## Watchtower
Watchtower is the graphics library, it is highly targeted at supporting a mix of platforms and graphics api, and will require multiple implementations built underneath a common abstraction design.

# Sandboxes
The main citadel source projects also contain a set of sandbox projects. Each project is a simple example of how to utilize various aspects of the engine.