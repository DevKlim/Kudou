#include "android_keys_utils.h"

Key godot_code_from_android_code(unsigned int p_code) {
	for (int i = 0; android_godot_code_pairs[i].android_code != AKEYCODE_MAX; i++) {
		if (android_godot_code_pairs[i].android_code == p_code) {
			return android_godot_code_pairs[i].godot_code;
		}
	}
	return Key::UNKNOWN;
}

KeyLocation godot_location_from_android_code(unsigned int p_code) {
	for (int i = 0; android_godot_location_pairs[i].android_code != AKEYCODE_MAX; i++) {
		if (android_godot_location_pairs[i].android_code == p_code) {
			return android_godot_location_pairs[i].godot_code;
		}
	}
	return KeyLocation::UNSPECIFIED;
}
