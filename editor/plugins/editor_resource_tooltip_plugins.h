#pragma once

#include "core/object/gdvirtual.gen.inc"
#include "core/object/ref_counted.h"
#include "scene/gui/control.h"

class Texture2D;
class TextureRect;
class VBoxContainer;

class EditorResourceTooltipPlugin : public RefCounted {
	GDCLASS(EditorResourceTooltipPlugin, RefCounted);

	void _thumbnail_ready(const String &p_path, const Ref<Texture2D> &p_preview, const Ref<Texture2D> &p_small_preview, const Variant &p_udata);

protected:
	static void _bind_methods();

	GDVIRTUAL1RC(bool, _handles, String)
	GDVIRTUAL3RC(Control *, _make_tooltip_for_path, String, Dictionary, Control *)

public:
	static VBoxContainer *make_default_tooltip(const String &p_resource_path);
	void request_thumbnail(const String &p_path, TextureRect *p_for_control) const;

	virtual bool handles(const String &p_resource_type) const;
	virtual Control *make_tooltip_for_path(const String &p_resource_path, const Dictionary &p_metadata, Control *p_base) const;
};

class EditorTextureTooltipPlugin : public EditorResourceTooltipPlugin {
	GDCLASS(EditorTextureTooltipPlugin, EditorResourceTooltipPlugin);

public:
	virtual bool handles(const String &p_resource_type) const override;
	virtual Control *make_tooltip_for_path(const String &p_resource_path, const Dictionary &p_metadata, Control *p_base) const override;
};

class EditorAudioStreamTooltipPlugin : public EditorResourceTooltipPlugin {
	GDCLASS(EditorAudioStreamTooltipPlugin, EditorResourceTooltipPlugin);

public:
	virtual bool handles(const String &p_resource_type) const override;
	virtual Control *make_tooltip_for_path(const String &p_resource_path, const Dictionary &p_metadata, Control *p_base) const override;
};
