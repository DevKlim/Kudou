#pragma once

// Use for constants etc. that need not be included as often as rendering_server.h
// to reduce dependencies and prevent slow compilation.

// This is a "cheap" include, and can be used from scene side code as well as servers.

// N.B. ONLY allow these defined in DEV_ENABLED builds, they will slow
// performance, and are only necessary to use for debugging.
#ifdef DEV_ENABLED

// Uncomment this define to produce debugging output for physics interpolation.
//#define RENDERING_SERVER_DEBUG_PHYSICS_INTERPOLATION

#endif // DEV_ENABLED
