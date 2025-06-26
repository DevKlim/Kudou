#include "webrtc_peer_connection_extension.h"

void WebRTCPeerConnectionExtension::_bind_methods() {
	GDVIRTUAL_BIND(_get_connection_state);
	GDVIRTUAL_BIND(_get_gathering_state);
	GDVIRTUAL_BIND(_get_signaling_state);
	GDVIRTUAL_BIND(_initialize, "p_config");
	GDVIRTUAL_BIND(_create_data_channel, "p_label", "p_config");
	GDVIRTUAL_BIND(_create_offer);
	GDVIRTUAL_BIND(_set_remote_description, "p_type", "p_sdp");
	GDVIRTUAL_BIND(_set_local_description, "p_type", "p_sdp");
	GDVIRTUAL_BIND(_add_ice_candidate, "p_sdp_mid_name", "p_sdp_mline_index", "p_sdp_name");
	GDVIRTUAL_BIND(_poll);
	GDVIRTUAL_BIND(_close);
}
