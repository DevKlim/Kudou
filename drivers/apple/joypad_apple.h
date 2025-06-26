#pragma once

#include "core/input/input.h"
#include "core/input/input_enums.h"

#define Key _QKey
#import <GameController/GameController.h>
#undef Key

@class GCController;
class RumbleContext;

struct GameController {
	int joy_id;
	GCController *controller;
	RumbleContext *rumble_context API_AVAILABLE(macos(11.0), ios(14.0), tvos(14.0)) = nil;
	NSInteger ff_effect_timestamp = 0;
	bool force_feedback = false;
	bool double_nintendo_joycon_layout = false;
	bool single_nintendo_joycon_layout = false;

	uint32_t axis_changed_mask = 0;
	static_assert(static_cast<uint32_t>(JoyAxis::MAX) < 32, "JoyAxis::MAX must be less than 32");
	double axis_value[(int)JoyAxis::MAX];

	GameController(int p_joy_id, GCController *p_controller);
	~GameController();
};

class JoypadApple {
private:
	id<NSObject> connect_observer = nil;
	id<NSObject> disconnect_observer = nil;
	HashMap<int, GameController *> joypads;
	HashMap<GCController *, int> controller_to_joy_id;

	GCControllerPlayerIndex get_free_player_index();

	void add_joypad(GCController *p_controller);
	void remove_joypad(GCController *p_controller);

public:
	JoypadApple();
	~JoypadApple();

	void joypad_vibration_start(GameController &p_joypad, float p_weak_magnitude, float p_strong_magnitude, float p_duration, uint64_t p_timestamp) API_AVAILABLE(macos(11.0), ios(14.0), tvos(14.0));
	void joypad_vibration_stop(GameController &p_joypad, uint64_t p_timestamp) API_AVAILABLE(macos(11.0), ios(14.0), tvos(14.0));

	void process_joypads();
};
