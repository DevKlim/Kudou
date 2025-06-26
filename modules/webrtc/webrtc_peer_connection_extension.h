#pragma once

#include "webrtc_peer_connection.h"

#include "core/extension/ext_wrappers.gen.inc"
#include "core/object/gdvirtual.gen.inc"

class WebRTCPeerConnectionExtension : public WebRTCPeerConnection {
	GDCLASS(WebRTCPeerConnectionExtension, WebRTCPeerConnection);

protected:
	static void _bind_methods();

public:
	/** GDExtension **/
	EXBIND0RC(ConnectionState, get_connection_state);
	EXBIND0RC(GatheringState, get_gathering_state);
	EXBIND0RC(SignalingState, get_signaling_state);
	EXBIND1R(Error, initialize, Dictionary);
	EXBIND2R(Ref<WebRTCDataChannel>, create_data_channel, String, Dictionary);
	EXBIND0R(Error, create_offer);
	EXBIND2R(Error, set_remote_description, String, String);
	EXBIND2R(Error, set_local_description, String, String);
	EXBIND3R(Error, add_ice_candidate, String, int, String);
	EXBIND0R(Error, poll);
	EXBIND0(close);

	WebRTCPeerConnectionExtension() {}
};
