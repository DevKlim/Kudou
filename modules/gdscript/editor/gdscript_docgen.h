#pragma once

#include "../gdscript_parser.h"

#include "core/doc_data.h"

class GDScriptDocGen {
	using GDP = GDScriptParser;
	using GDType = GDP::DataType;

	static HashMap<String, String> singletons; // Script path to singleton name.

	static String _get_script_name(const String &p_path);
	static String _get_class_name(const GDP::ClassNode &p_class);
	static void _doctype_from_gdtype(const GDType &p_gdtype, String &r_type, String &r_enum, bool p_is_return = false);
	static String _docvalue_from_variant(const Variant &p_variant, int p_recursion_level = 1);
	static void _generate_docs(GDScript *p_script, const GDP::ClassNode *p_class);

public:
	static void generate_docs(GDScript *p_script, const GDP::ClassNode *p_class);
	static void doctype_from_gdtype(const GDType &p_gdtype, String &r_type, String &r_enum, bool p_is_return = false);
	static String docvalue_from_expression(const GDP::ExpressionNode *p_expression);
};
