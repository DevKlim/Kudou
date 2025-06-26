#pragma once

#include "openxr_extension_wrapper.h"

class OpenXRCompositionLayerDepthExtension : public OpenXRExtensionWrapper {
	GDCLASS(OpenXRCompositionLayerDepthExtension, OpenXRExtensionWrapper);

protected:
	static void _bind_methods() {}

public:
	static OpenXRCompositionLayerDepthExtension *get_singleton();

	OpenXRCompositionLayerDepthExtension();
	virtual ~OpenXRCompositionLayerDepthExtension() override;

	virtual HashMap<String, bool *> get_requested_extensions() override;
	bool is_available();
	virtual int get_composition_layer_count() override;
	virtual XrCompositionLayerBaseHeader *get_composition_layer(int p_index) override;
	virtual int get_composition_layer_order(int p_index) override;

private:
	static OpenXRCompositionLayerDepthExtension *singleton;

	bool available = false;
};
