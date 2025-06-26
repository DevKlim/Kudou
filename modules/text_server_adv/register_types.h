#pragma once

#ifdef GDEXTENSION
#include <godot_cpp/core/class_db.hpp>
using namespace godot;
#elif defined(GODOT_MODULE)
#include "modules/register_module_types.h"
#endif

void initialize_text_server_adv_module(ModuleInitializationLevel p_level);
void uninitialize_text_server_adv_module(ModuleInitializationLevel p_level);
