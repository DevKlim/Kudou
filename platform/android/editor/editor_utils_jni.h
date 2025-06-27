#pragma once

#include <jni.h>

extern "C" {
JNIEXPORT void JNICALL Java_org_godotengine_godot_editor_utils_EditorUtils_runScene(JNIEnv *p_env, jclass, jstring p_scene, jobjectArray p_scene_args);
}
