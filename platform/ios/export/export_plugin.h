#pragma once

#include "editor/export/editor_export_platform_apple_embedded.h"

class EditorExportPlatformIOS : public EditorExportPlatformAppleEmbedded {
	GDCLASS(EditorExportPlatformIOS, EditorExportPlatformAppleEmbedded);

	static Vector<String> device_types;

	virtual String get_platform_name() const override { return "ios"; }
	virtual String get_sdk_name() const override { return "iphoneos"; }
	virtual const Vector<String> get_device_types() const override { return device_types; }

	virtual String get_minimum_deployment_target() const override { return "14.0"; }

	virtual Vector<IconInfo> get_icon_infos() const override;

	virtual void get_export_options(List<ExportOption> *r_options) const override;
	virtual bool has_valid_export_configuration(const Ref<EditorExportPreset> &p_preset, String &r_error, bool &r_missing_templates, bool p_debug = false) const override;

	virtual Error _export_loading_screen_file(const Ref<EditorExportPreset> &p_preset, const String &p_dest_dir) override;
	virtual Error _export_icons(const Ref<EditorExportPreset> &p_preset, const String &p_iconset_dir) override;
	virtual HashMap<String, Variant> get_custom_project_settings(const Ref<EditorExportPreset> &p_preset) const override;

public:
	virtual String get_name() const override { return "iOS"; }
	virtual String get_os_name() const override { return "iOS"; }

	virtual void get_platform_features(List<String> *r_features) const override {
		EditorExportPlatformAppleEmbedded::get_platform_features(r_features);
		r_features->push_back("ios");
	}

	EditorExportPlatformIOS();
	~EditorExportPlatformIOS();
};
