#pragma once

#include "core/string/ustring.h"
#include "core/templates/vector.h"

Vector<String> get_editor_locales();
void load_editor_translations(const String &p_locale);
void load_property_translations(const String &p_locale);
void load_doc_translations(const String &p_locale);
void load_extractable_translations(const String &p_locale);
Vector<Vector<String>> get_extractable_message_list();
