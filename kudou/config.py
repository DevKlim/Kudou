def can_build(env, platform):
    return True


def configure(env):
    platform = env["platform"]
    if platform == "windows":
        # The include path must be the parent of the 'include' directory.
        env.Append(CPPPATH=["#thirdparty/cef_binary"])
        
        # Construct the library name dynamically.
        arch = env["arch"]
        target = env["target"]
        
        # Handle potential variations in target names.
        if target == "template_debug":
            target_str = "debug"
        elif target == "template_release":
            target_str = "release"
        else:
            target_str = "editor"

        cef_lib_path = f"#thirdparty/cef_binary/libs/libcef.lib.{platform}.{target_str}.{arch}.lib"
        
        # Add the full path to the library to prevent name mangling.
        env.Append(LIBS=[
            env.File(cef_lib_path),
            env.File("#thirdparty/cef_binary/libs/libcef_dll_wrapper.lib")
        ])
        env.Append(CPPDEFINES=["CEF_USE_SANDBOX=0", "CEF_ENABLE_PRINTING=0"])
    elif platform == "linux":
        # The include path must be the parent of the 'include' directory.
        env.Append(CPPPATH=["#thirdparty/cef_binary"])
        # Assuming 'libs' is the correct folder on Linux too for consistency.
        env.Append(LIBPATH=["#thirdparty/cef_binary/libs"])
        env.Append(LIBS=["cef", "cef_dll_wrapper"])
        env.Append(CPPDEFINES=["CEF_USE_SANDBOX=0", "CEF_ENABLE_PRINTING=0"])
    elif platform == "osx":
        # The include path must be the parent of the 'include'directory.
        env.Append(CPPPATH=["#thirdparty/cef_binary"])
        # Assuming 'libs' is the correct folder on macOS too for consistency.
        env.Append(LIBPATH=["#thirdparty/cef_binary/libs"])
        env.Append(LIBS=["cef", "cef_dll_wrapper"])
        env.Append(CPPDEFINES=["CEF_USE_SANDBOX=0", "CEF_ENABLE_PRINTING=0"])
