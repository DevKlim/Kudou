#pragma once

#include "editor/editor_translation_parser.h"

class PackedSceneEditorTranslationParserPlugin : public EditorTranslationParserPlugin {
	GDCLASS(PackedSceneEditorTranslationParserPlugin, EditorTranslationParserPlugin);

	// Scene Node's properties that contain translation strings.
	HashSet<String> lookup_properties;
	// Properties from specific Nodes that should be ignored.
	HashMap<String, Vector<String>> exception_list;

public:
	virtual Error parse_file(const String &p_path, Vector<Vector<String>> *r_translations) override;
	bool match_property(const String &p_property_name, const String &p_node_type);
	virtual void get_recognized_extensions(List<String> *r_extensions) const override;

	PackedSceneEditorTranslationParserPlugin();
};
