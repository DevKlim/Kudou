#include "editor_utils_jni.h"

#include "jni_utils.h"

#ifdef TOOLS_ENABLED
#include "editor/gui/editor_run_bar.h"
#include "main/main.h"
#endif

extern "C" {
JNIEXPORT void JNICALL Java_org_godotengine_godot_editor_utils_EditorUtils_runScene(JNIEnv *p_env, jclass, jstring p_scene, jobjectArray p_scene_args) {
#ifdef TOOLS_ENABLED
	Vector<String> scene_args;
	jint length = p_env->GetArrayLength(p_scene_args);
	for (jint i = 0; i < length; ++i) {
		jstring j_arg = (jstring)p_env->GetObjectArrayElement(p_scene_args, i);
		String arg = jstring_to_string(j_arg, p_env);
		scene_args.push_back(arg);
		p_env->DeleteLocalRef(j_arg);
	}

	String scene = jstring_to_string(p_scene, p_env);

	EditorRunBar *editor_run_bar = EditorRunBar::get_singleton();
	if (editor_run_bar != nullptr) {
		if (scene.is_empty()) {
			editor_run_bar->play_main_scene(false);
		} else {
			editor_run_bar->play_custom_scene(scene, scene_args);
		}
	} else {
		List<String> args;

		for (const String &a : Main::get_forwardable_cli_arguments(Main::CLI_SCOPE_PROJECT)) {
			args.push_back(a);
		}

		for (const String &arg : scene_args) {
			args.push_back(arg);
		}

		if (!scene.is_empty()) {
			args.push_back("--scene");
			args.push_back(scene);
		}

		Error err = OS::get_singleton()->create_instance(args);
		ERR_FAIL_COND(err);
	}
#endif
}
}
