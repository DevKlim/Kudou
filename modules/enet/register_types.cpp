#include "register_types.h"

#include "enet_connection.h"
#include "enet_multiplayer_peer.h"
#include "enet_packet_peer.h"

#include "core/error/error_macros.h"

static bool enet_ok = false;

void initialize_enet_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	if (enet_initialize() != 0) {
		ERR_PRINT("ENet initialization failure");
	} else {
		enet_ok = true;
	}

	GDREGISTER_CLASS(ENetMultiplayerPeer);
	GDREGISTER_ABSTRACT_CLASS(ENetPacketPeer);
	GDREGISTER_CLASS(ENetConnection);
}

void uninitialize_enet_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	if (enet_ok) {
		enet_deinitialize();
	}
}
