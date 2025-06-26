#pragma once

#include "webrtc_data_channel.h"

class WebRTCPeerConnection : public RefCounted {
	GDCLASS(WebRTCPeerConnection, RefCounted);

public:
	enum ConnectionState {
		STATE_NEW,
		STATE_CONNECTING,
		STATE_CONNECTED,
		STATE_DISCONNECTED,
		STATE_FAILED,
		STATE_CLOSED
	};

	enum GatheringState {
		GATHERING_STATE_NEW,
		GATHERING_STATE_GATHERING,
		GATHERING_STATE_COMPLETE,
	};

	enum SignalingState {
		SIGNALING_STATE_STABLE,
		SIGNALING_STATE_HAVE_LOCAL_OFFER,
		SIGNALING_STATE_HAVE_REMOTE_OFFER,
		SIGNALING_STATE_HAVE_LOCAL_PRANSWER,
		SIGNALING_STATE_HAVE_REMOTE_PRANSWER,
		SIGNALING_STATE_CLOSED,
	};

private:
	static StringName default_extension;

protected:
	static void _bind_methods();

public:
	static void set_default_extension(const StringName &p_name);

	virtual ConnectionState get_connection_state() const = 0;
	virtual GatheringState get_gathering_state() const = 0;
	virtual SignalingState get_signaling_state() const = 0;

	virtual Error initialize(Dictionary p_config = Dictionary()) = 0;
	virtual Ref<WebRTCDataChannel> create_data_channel(String p_label, Dictionary p_options = Dictionary()) = 0;
	virtual Error create_offer() = 0;
	virtual Error set_remote_description(String type, String sdp) = 0;
	virtual Error set_local_description(String type, String sdp) = 0;
	virtual Error add_ice_candidate(String sdpMidName, int sdpMlineIndexName, String sdpName) = 0;
	virtual Error poll() = 0;
	virtual void close() = 0;

	static WebRTCPeerConnection *create(bool p_notify_postinitialize = true);

	WebRTCPeerConnection();
	~WebRTCPeerConnection();
};

VARIANT_ENUM_CAST(WebRTCPeerConnection::ConnectionState);
VARIANT_ENUM_CAST(WebRTCPeerConnection::GatheringState);
VARIANT_ENUM_CAST(WebRTCPeerConnection::SignalingState);
