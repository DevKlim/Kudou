// Wrapper needed to set the required rpcndr version for MinGW compatibility.
// Since we're compiling thirdparty code in a Godot SCons environment with
// warnings enabled, we also need to silence them manually.

#include "rendering_device_driver_d3d12.h" // For __REQUIRED_RPCNDR_H_VERSION__.

GODOT_GCC_WARNING_PUSH_AND_IGNORE("-Wmaybe-uninitialized")

#include <D3D12MemAlloc.cpp>

GODOT_GCC_WARNING_POP
