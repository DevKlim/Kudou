#pragma once

#include "editor/plugins/game_view_plugin.h"

class EmbeddedProcessMacOS;

class GameViewDebuggerMacOS : public GameViewDebugger {
	GDCLASS(GameViewDebuggerMacOS, GameViewDebugger);

	EmbeddedProcessMacOS *embedded_process = nullptr;

	/// Message handler function for capture.

	/// @brief A function pointer to the message handler function.
	typedef bool (GameViewDebuggerMacOS::*ParseMessageFunc)(const Array &p_args);

	/// @brief A map of message handlers.
	static HashMap<String, ParseMessageFunc> parse_message_handlers;

	/// @brief Initialize the message handlers.
	static void _init_capture_message_handlers();

	bool _msg_set_context_id(const Array &p_args);
	bool _msg_cursor_set_shape(const Array &p_args);
	bool _msg_mouse_set_mode(const Array &p_args);
	bool _msg_window_set_ime_active(const Array &p_args);
	bool _msg_window_set_ime_position(const Array &p_args);
	bool _msg_joy_start(const Array &p_args);
	bool _msg_joy_stop(const Array &p_args);

public:
	virtual bool capture(const String &p_message, const Array &p_data, int p_session) override;

	GameViewDebuggerMacOS(EmbeddedProcessMacOS *p_embedded_process);
};

class GameViewPluginMacOS : public GameViewPluginBase {
	GDCLASS(GameViewPluginMacOS, GameViewPluginBase);

public:
	GameViewPluginMacOS();
};

extern "C" void register_game_view_plugin();
