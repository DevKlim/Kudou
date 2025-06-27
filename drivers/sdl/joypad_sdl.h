#pragma once

#include "core/input/input.h"
#include "core/os/thread.h"

typedef uint32_t SDL_JoystickID;
typedef struct HWND__ *HWND;

class JoypadSDL {
public:
	JoypadSDL();
	~JoypadSDL();

	static JoypadSDL *get_singleton();

	Error initialize();
	void process_events();
#ifdef WINDOWS_ENABLED
	void setup_sdl_helper_window(HWND p_hwnd);
#endif

private:
	struct Joypad {
		bool attached = false;
		StringName guid;

		SDL_JoystickID sdl_instance_idx;

		bool supports_force_feedback = false;
		uint64_t ff_effect_timestamp = 0;
	};

	static JoypadSDL *singleton;

	Joypad joypads[Input::JOYPADS_MAX];
	HashMap<SDL_JoystickID, int> sdl_instance_id_to_joypad_id;

	void close_joypad(int p_pad_idx);
};
