#pragma once

#include "core/string/ustring.h"
#include "core/templates/hash_map.h"
#include "core/templates/list.h"
#include "core/variant/array.h"
#include "servers/display_server.h"

#include <objbase.h>
#include <sapi.h>
#include <winnls.h>
#include <cwchar>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class TTS_Windows {
	List<DisplayServer::TTSUtterance> queue;
	ISpVoice *synth = nullptr;
	bool paused = false;
	struct UTData {
		Char16String string;
		int offset;
		int id;
	};
	HashMap<uint32_t, UTData> ids;
	bool update_requested = false;

	static void __stdcall speech_event_callback(WPARAM wParam, LPARAM lParam);

	static TTS_Windows *singleton;

public:
	static TTS_Windows *get_singleton();

	bool is_speaking() const;
	bool is_paused() const;
	Array get_voices() const;

	void speak(const String &p_text, const String &p_voice, int p_volume = 50, float p_pitch = 1.f, float p_rate = 1.f, int p_utterance_id = 0, bool p_interrupt = false);
	void pause();
	void resume();
	void stop();

	void process_events();

	TTS_Windows();
	~TTS_Windows();
};
