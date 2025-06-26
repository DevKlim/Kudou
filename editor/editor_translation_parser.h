#pragma once

#include "core/object/gdvirtual.gen.inc"
#include "core/object/ref_counted.h"
#include "core/variant/typed_array.h"

class EditorTranslationParserPlugin : public RefCounted {
	GDCLASS(EditorTranslationParserPlugin, RefCounted);

protected:
	static void _bind_methods();

	GDVIRTUAL1R(TypedArray<PackedStringArray>, _parse_file, String)
	GDVIRTUAL0RC(Vector<String>, _get_recognized_extensions)

#ifndef DISABLE_DEPRECATED
	GDVIRTUAL3_COMPAT(_parse_file_bind_compat_99297, _parse_file, String, TypedArray<String>, TypedArray<Array>)
#endif

public:
	virtual Error parse_file(const String &p_path, Vector<Vector<String>> *r_translations);
	virtual void get_recognized_extensions(List<String> *r_extensions) const;
};

class EditorTranslationParser {
	static EditorTranslationParser *singleton;

public:
	enum ParserType {
		STANDARD, // GDScript, CSharp, ...
		CUSTOM // User-defined parser plugins. This will override standard parsers if the same extension type is defined.
	};

	static EditorTranslationParser *get_singleton();

	Vector<Ref<EditorTranslationParserPlugin>> standard_parsers;
	Vector<Ref<EditorTranslationParserPlugin>> custom_parsers;

	void get_recognized_extensions(List<String> *r_extensions) const;
	bool can_parse(const String &p_extension) const;
	Ref<EditorTranslationParserPlugin> get_parser(const String &p_extension) const;
	void add_parser(const Ref<EditorTranslationParserPlugin> &p_parser, ParserType p_type);
	void remove_parser(const Ref<EditorTranslationParserPlugin> &p_parser, ParserType p_type);
	void clean_parsers();

	~EditorTranslationParser();
};
