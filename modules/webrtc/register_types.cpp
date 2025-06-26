#include "register_types.h"

#include "webrtc_data_channel.h"
#include "webrtc_data_channel_extension.h"
#include "webrtc_multiplayer_peer.h"
#include "webrtc_peer_connection.h"
#include "webrtc_peer_connection_extension.h"

#include "core/config/project_settings.h"

void initialize_webrtc_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GLOBAL_DEF(PropertyInfo(Variant::INT, "network/limits/webrtc/max_channel_in_buffer_kb", PROPERTY_HINT_RANGE, "2,4096,1,or_greater"), 64);

	ClassDB::register_custom_instance_class<WebRTCPeerConnection>();
	GDREGISTER_CLASS(WebRTCPeerConnectionExtension);

	GDREGISTER_ABSTRACT_CLASS(WebRTCDataChannel);
	GDREGISTER_CLASS(WebRTCDataChannelExtension);

	GDREGISTER_CLASS(WebRTCMultiplayerPeer);
}

void uninitialize_webrtc_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}
