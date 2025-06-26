#pragma once

#ifdef DEBUG_ENABLED

#include "core/object/class_db.h"

void class_db_api_to_json(const String &p_output_file, ClassDB::APIType p_api);

#endif // DEBUG_ENABLED
