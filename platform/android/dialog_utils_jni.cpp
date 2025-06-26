#include "dialog_utils_jni.h"

#include "display_server_android.h"
#include "jni_utils.h"

extern "C" {

JNIEXPORT void JNICALL Java_org_godotengine_godot_utils_DialogUtils_dialogCallback(JNIEnv *env, jclass clazz, jint p_button_index) {
	DisplayServerAndroid *ds = (DisplayServerAndroid *)DisplayServer::get_singleton();
	if (ds) {
		ds->emit_dialog_callback(p_button_index);
	}
}

JNIEXPORT void JNICALL Java_org_godotengine_godot_utils_DialogUtils_inputDialogCallback(JNIEnv *env, jclass clazz, jstring p_text) {
	DisplayServerAndroid *ds = (DisplayServerAndroid *)DisplayServer::get_singleton();
	if (ds) {
		String text = jstring_to_string(p_text, env);
		ds->emit_input_dialog_callback(text);
	}
}
}
