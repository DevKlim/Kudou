#pragma once

#include "core/config/project_settings.h"
#include "core/string/ustring.h"
#include "core/templates/hash_map.h"
#include "core/variant/array.h"
#include "servers/display_server.h"

#include <jni.h>

class TTS_Android {
	static bool initialized;
	static jobject tts;
	static jclass cls;

	static jmethodID _init;
	static jmethodID _is_speaking;
	static jmethodID _is_paused;
	static jmethodID _get_voices;
	static jmethodID _speak;
	static jmethodID _pause_speaking;
	static jmethodID _resume_speaking;
	static jmethodID _stop_speaking;

	static HashMap<int, Char16String> ids;

	static void initialize_tts();

public:
	static void setup(jobject p_tts);
	static void terminate();
	static void _java_utterance_callback(int p_event, int p_id, int p_pos);

	static bool is_speaking();
	static bool is_paused();
	static Array get_voices();
	static void speak(const String &p_text, const String &p_voice, int p_volume, float p_pitch, float p_rate, int p_utterance_id, bool p_interrupt);
	static void pause();
	static void resume();
	static void stop();
};
