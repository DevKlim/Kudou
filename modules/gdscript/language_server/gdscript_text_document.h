#pragma once

#include "godot_lsp.h"

#include "core/io/file_access.h"
#include "core/object/ref_counted.h"

class GDScript;

class GDScriptTextDocument : public RefCounted {
	GDCLASS(GDScriptTextDocument, RefCounted)
protected:
	static void _bind_methods();

	Ref<FileAccess> file_checker;

	Array native_member_completions;

private:
	Array find_symbols(const LSP::TextDocumentPositionParams &p_location, List<const LSP::DocumentSymbol *> &r_list);
	LSP::TextDocumentItem load_document_item(const Variant &p_param);
	void notify_client_show_symbol(const LSP::DocumentSymbol *symbol);

public:
	void didOpen(const Variant &p_param);
	void didClose(const Variant &p_param);
	void didChange(const Variant &p_param);
	void willSaveWaitUntil(const Variant &p_param);
	void didSave(const Variant &p_param);

	void reload_script(Ref<GDScript> p_to_reload_script);
	void sync_script_content(const String &p_path, const String &p_content);
	void show_native_symbol_in_editor(const String &p_symbol_id);

	Variant nativeSymbol(const Dictionary &p_params);
	Array documentSymbol(const Dictionary &p_params);
	Array completion(const Dictionary &p_params);
	Dictionary resolve(const Dictionary &p_params);
	Dictionary rename(const Dictionary &p_params);
	Variant prepareRename(const Dictionary &p_params);
	Array references(const Dictionary &p_params);
	Array foldingRange(const Dictionary &p_params);
	Array codeLens(const Dictionary &p_params);
	Array documentLink(const Dictionary &p_params);
	Array colorPresentation(const Dictionary &p_params);
	Variant hover(const Dictionary &p_params);
	Array definition(const Dictionary &p_params);
	Variant declaration(const Dictionary &p_params);
	Variant signatureHelp(const Dictionary &p_params);

	void initialize();

	GDScriptTextDocument();
};
