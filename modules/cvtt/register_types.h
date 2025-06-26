#pragma once

#ifdef TOOLS_ENABLED

#include "modules/register_module_types.h"

void initialize_cvtt_module(ModuleInitializationLevel p_level);
void uninitialize_cvtt_module(ModuleInitializationLevel p_level);

#endif // TOOLS_ENABLED
