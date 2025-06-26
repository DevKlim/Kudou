#pragma once

// This extension gives us access to the possible display refresh rates
// supported by the HMD.
// While this is an FB extension it has been adopted by most runtimes and
// will likely become core in the near future.

#include "../openxr_api.h"
#include "../util.h"
#include "openxr_extension_wrapper.h"

class OpenXRDisplayRefreshRateExtension : public OpenXRExtensionWrapper {
	GDCLASS(OpenXRDisplayRefreshRateExtension, OpenXRExtensionWrapper);

protected:
	static void _bind_methods() {}

public:
	static OpenXRDisplayRefreshRateExtension *get_singleton();

	OpenXRDisplayRefreshRateExtension();
	virtual ~OpenXRDisplayRefreshRateExtension() override;

	virtual HashMap<String, bool *> get_requested_extensions() override;

	virtual void on_instance_created(const XrInstance p_instance) override;
	virtual void on_instance_destroyed() override;
	virtual bool on_event_polled(const XrEventDataBuffer &event) override;

	float get_refresh_rate() const;
	void set_refresh_rate(float p_refresh_rate);

	Array get_available_refresh_rates() const;

private:
	static OpenXRDisplayRefreshRateExtension *singleton;

	bool display_refresh_rate_ext = false;

	// OpenXR API call wrappers
	EXT_PROTO_XRRESULT_FUNC4(xrEnumerateDisplayRefreshRatesFB, (XrSession), session, (uint32_t), displayRefreshRateCapacityInput, (uint32_t *), displayRefreshRateCountOutput, (float *), displayRefreshRates);
	EXT_PROTO_XRRESULT_FUNC2(xrGetDisplayRefreshRateFB, (XrSession), session, (float *), display_refresh_rate);
	EXT_PROTO_XRRESULT_FUNC2(xrRequestDisplayRefreshRateFB, (XrSession), session, (float), display_refresh_rate);
};
