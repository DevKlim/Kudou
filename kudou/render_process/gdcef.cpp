#include "gdcef.hpp"
#include "gdbrowser.hpp"
#include "helper_config.hpp"
#include "helper_files.hpp"

#include "core/config/project_settings.h"
#include "core/error/error_list.h"
#include "core/object/class_db.h"
#include "core/os/os.h"

// Guards for CEF conflicts are now in gdbrowser.hpp (via ad_blocker.hpp)

#include "base/cef_callback.h"
#include "cef_version.h"
#include "wrapper/cef_closure_task.h"

#ifdef _OPENMP
#include <omp.h>
#endif

#if defined(_WIN32)
#define SUBPROCESS_NAME "gdCefRenderProcess.exe"
#define NEEDED_LIBRARIES                                                \
	"libcef.dll", "libgdcef.dll", "vulkan-1.dll", "vk_swiftshader.dll", \
			"libGLESv2.dll", "libEGL.dll"
#elif defined(__linux__)
#define SUBPROCESS_NAME "gdCefRenderProcess"
#define NEEDED_LIBRARIES                                                  \
	"libcef.so", "libgdcef.so", "libvulkan.so.1", "libvk_swiftshader.so", \
			"libGLESv2.so", "libEGL.so"
#elif defined(__APPLE__)
#define SUBPROCESS_NAME "cefsimple.app"
#define NEEDED_LIBRARIES "libgdcef.dylib"
#else
#error \
		"Undefined path for the Godot's CEF sub process for this architecture"
#endif

#define CEF_ARTIFACTS_FOLDER "res://cef_artifacts"

static void configureCEF(fs::path const &folder,
		CefSettings &cef_settings,
		CefWindowInfo &window_info,
		Dictionary config);

static bool sanity_checks(fs::path const &folder) {
#if defined(__APPLE__)
	std::string lib_dir = SUBPROCESS_NAME
			"/Contents/Frameworks/Chromium Embedded Framework.framework/Libraries/";
	std::string res_dir = SUBPROCESS_NAME
			"/Contents/Frameworks/Chromium Embedded Framework.framework/Resources/";
	const std::vector<std::string> files = {
		SUBPROCESS_NAME,
		NEEDED_LIBRARIES,
		lib_dir + "libvk_swiftshader.dylib",
		lib_dir + "libGLESv2.dylib",
		lib_dir + "libEGL.dylib",
		res_dir + "icudtl.dat",
		res_dir + "chrome_100_percent.pak",
		res_dir + "chrome_200_percent.pak",
		res_dir + "resources.pak",
	};
#else
	const std::vector<std::string> files = { SUBPROCESS_NAME,
		NEEDED_LIBRARIES,
		"icudtl.dat",
		"chrome_100_percent.pak",
		"chrome_200_percent.pak",
		"resources.pak",
		"v8_context_snapshot.bin" };
#endif

	return are_valid_files(folder, files);
}

void GDCef::_bind_methods() {
	GDCEF_DEBUG("");

	ClassDB::bind_method(D_METHOD("initialize", "config"), &GDCef::initialize);
	ClassDB::bind_method(D_METHOD("get_full_version"), &GDCef::version);
	//ClassDB::bind_method(D_METHOD("get_version_part"), &GDCef::versionPart);
	ClassDB::bind_method(D_METHOD("create_browser"), &GDCef::createBrowser);
	ClassDB::bind_method(D_METHOD("shutdown"), &GDCef::shutdown);
	ClassDB::bind_method(D_METHOD("is_alive"), &GDCef::isAlive);
	ClassDB::bind_method(D_METHOD("get_error"), &GDCef::getError);
	ClassDB::bind_method(D_METHOD("log_info", "message"), &GDCef::log_info);
	ClassDB::bind_method(D_METHOD("log_warning", "message"), &GDCef::log_warning);
	ClassDB::bind_method(D_METHOD("log_error", "message"), &GDCef::log_error);
	ClassDB::bind_method(D_METHOD("log_fatal", "message"), &GDCef::log_fatal);
}

void GDCef::_init() {
	GDCEF_DEBUG("");
}

void GDCef::_exit_tree() {
	GDCEF_DEBUG("");
	shutdown();
}

void GDCef::shutdown() {
	GDCEF_DEBUG("");

	if (m_impl != nullptr) {
		GDCEF_DEBUG("Closing all browsers");
		m_impl->closeAllBrowsers(true);

		m_impl = nullptr;

		GDCEF_DEBUG("CefQuitMessageLoop");
		CefQuitMessageLoop();
	}
}

bool GDCef::initialize(Dictionary config) {
#ifdef _OPENMP
#pragma omp parallel
	{
#pragma omp single
		GDCEF_DEBUG("OpenMP number of threads = " << omp_get_num_threads());
	}
#else
	GDCEF_DEBUG("OpenMP is not enabled");
#endif

	if (m_impl != nullptr) {
		GDCEF_ERROR("Already initialized");
		return false;
	}
	m_impl = new GDCef::Impl(*this);
	assert((m_impl != nullptr) && "Failed allocating GDCef");

	fs::path cef_folder_path;

	if (OS::get_singleton()->has_feature("editor")) {
		std::string cef_artifacts_folder =
				getConfig(config, "artifacts", std::string(CEF_ARTIFACTS_FOLDER));

		if (cef_artifacts_folder.rfind("res://", 0) == 0) {
			cef_folder_path = ProjectSettings::get_singleton()->globalize_path(cef_artifacts_folder.c_str()).utf8().get_data();
		} else {
			cef_folder_path =
					std::filesystem::current_path() / cef_artifacts_folder;
		}
		GDCEF_DEBUG("Launching CEF from Godot editor");
		GDCEF_DEBUG("Path where your project Godot files shall be located: "
				<< cef_folder_path);
	} else {
		String default_path_godot_str = OS::get_singleton()->get_executable_path().get_base_dir().path_join(CEF_ARTIFACTS_FOLDER);
		fs::path default_path = default_path_godot_str.utf8().get_data();
		cef_folder_path = getConfig(config, "exported_artifacts", default_path);
		GDCEF_DEBUG("Launching CEF from your executable");
		GDCEF_DEBUG("Path where your application files shall be located: "
				<< cef_folder_path);
	}

	if (!sanity_checks(cef_folder_path)) {
		GDCEF_ERROR("Error: at least one CEF artifact was not found in folder "
				<< cef_folder_path
				<< ". Your gdCEF node will still be present but disabled.");
		m_impl = nullptr;
		return false;
	}

	configureCEF(cef_folder_path, m_cef_settings, m_window_info, config);
	m_browsers_settings.enable_media_stream =
			getConfig(config, "enable_media_stream", false);
	m_browsers_settings.remote_allow_origin =
			getConfig(config, "remote_allow_origin", std::string{});
	m_browsers_settings.enable_ad_block =
			getConfig(config, "enable_ad_block", true);
	m_browsers_settings.custom_patterns =
			getConfig(config, "ad_block_patterns", Array());
	m_browsers_settings.user_gesture_required =
			getConfig(config, "user_gesture_required", true);
	m_browsers_settings.user_agent =
			getConfig(config, "user_agent", std::string{});

	CefMainArgs args;
	GDCEF_DEBUG("CefInitialize");
	if (!CefInitialize(args, m_cef_settings, new GDCef::Impl(*this), nullptr)) {
		GDCEF_ERROR("CEF failed its initialization. Your gdCEF node will still "
					"be present but disabled.");
		m_impl = nullptr;
		return false;
	}
	GDCEF_DEBUG("CefInitialize done with success");
	return true;
}

bool GDCef::isAlive() {
	return m_impl != nullptr;
}

/*
int GDCef::versionPart(int entry) {
	return cef_version_info(entry);
}
*/

String GDCef::version() {
	return CEF_VERSION;
}

String GDCef::getError() {
	std::string err = m_error.str();
	m_error.clear();
	return String(err.c_str());
}

void GDCef::log_info(String message) {
	LOG(INFO) << message.utf8().get_data();
}

void GDCef::log_warning(String message) {
	LOG(WARNING) << message.utf8().get_data();
}

void GDCef::log_error(String message) {
	LOG(ERROR) << message.utf8().get_data();
}

void GDCef::log_fatal(String message) {
	LOG(FATAL) << message.utf8().get_data();
}

void GDCef::_process(double) {
	if (m_impl != nullptr) {
		CefDoMessageLoopWork();
	}
}

static void configureCEF(fs::path const &folder,
		CefSettings &cef_settings,
		CefWindowInfo &window_info,
		Dictionary config) {
#if !defined(__APPLE__)
	fs::path sub_process_path =
			getConfig(config, "browser_subprocess_path", folder / SUBPROCESS_NAME);
	GDCEF_DEBUG("Setting SubProcess path: " << sub_process_path.string());
	CefString(&cef_settings.browser_subprocess_path)
			.FromString(sub_process_path.string());
#else
	fs::path main_bundle_path = folder / SUBPROCESS_NAME;
	fs::path subprocess_path = main_bundle_path /
			"Contents/Frameworks/cefsimple "
			"Helper.app/Contents/MacOS/cefsimple Helper";
	CefString(&cef_settings.main_bundle_path)
			.FromString(main_bundle_path.string());
	CefString(&cef_settings.browser_subprocess_path)
			.FromString(subprocess_path.string());
	GDCEF_DEBUG("Setting SubProcess path: " << main_bundle_path.string());
#endif

	fs::path root_cache =
			getConfig(config, "root_cache_path", folder / "cache");
	GDCEF_DEBUG("Setting root cache path: " << root_cache.string());
	CefString(&cef_settings.root_cache_path).FromString(root_cache.string());

	const bool incognito = getConfig(config, "incognito", false);
	if (incognito) {
		GDCEF_DEBUG("Setting cache path as incognito");
		CefString(&cef_settings.cache_path).FromString("");
	} else {
		fs::path sub_process_cache =
				getConfig(config, "cache_path", root_cache);
		GDCEF_DEBUG("Setting cache path: " << sub_process_cache.string());
		CefString(&cef_settings.cache_path)
				.FromString(sub_process_cache.string());
	}

	fs::path locales_path =
			getConfig(config, "locales_path", folder / "locales");
	GDCEF_DEBUG("Setting locales path: " << locales_path.string());
	CefString(&cef_settings.locales_dir_path).FromString(locales_path.string());

	std::string locale = getConfig(config, "locale", std::string("en-US"));
	CefString(&cef_settings.locale).FromString(locale);
	GDCEF_DEBUG("Default locale: " << locale);

	fs::path log_file_path = getConfig(config, "log_file", folder / "debug.log");
	CefString(&cef_settings.log_file).FromString(log_file_path.string());
	std::string logString =
			getConfig(config, "log_severity", std::string("warning"));
	if (logString == "verbose") {
		cef_settings.log_severity = LOGSEVERITY_VERBOSE;
	} else if (logString == "info") {
		cef_settings.log_severity = LOGSEVERITY_INFO;
	} else if (logString == "warning") {
		cef_settings.log_severity = LOGSEVERITY_WARNING;
	} else if (logString == "error") {
		cef_settings.log_severity = LOGSEVERITY_ERROR;
	} else if (logString == "fatal") {
		cef_settings.log_severity = LOGSEVERITY_FATAL;
	}

	cef_settings.windowless_rendering_enabled = true;
	window_info.SetAsWindowless(0);
	window_info.shared_texture_enabled = false;
	cef_settings.no_sandbox = true;
	cef_settings.command_line_args_disabled = true;
	cef_settings.remote_debugging_port =
			getConfig(config, "remote_debugging_port", 7777);
	cef_settings.uncaught_exception_stack_size =
			getConfig(config, "exception_stack_size", 5);
	cef_settings.persist_session_cookies =
			getConfig(config, "persist_session_cookies", true);
	cef_settings.multi_threaded_message_loop = 0;
}

static void configureBrowser(CefBrowserSettings &browser_settings,
		Dictionary config) {
	browser_settings.windowless_frame_rate =
			getConfig(config, "frame_rate", 30);
	GDCEF_DEBUG("Using windowless_frame_rate: "
			<< int(browser_settings.windowless_frame_rate));

	browser_settings.javascript =
			getConfig(config, "javascript", STATE_ENABLED);
	GDCEF_DEBUG("Using javascript: " << int(browser_settings.javascript));

	browser_settings.javascript_close_windows =
			getConfig(config, "javascript_close_windows", STATE_DISABLED);
	GDCEF_DEBUG("Using javascript_close_windows: "
			<< int(browser_settings.javascript_close_windows));

	browser_settings.javascript_access_clipboard =
			getConfig(config, "javascript_access_clipboard", STATE_DISABLED);
	GDCEF_DEBUG("Using javascript_access_clipboard: "
			<< int(browser_settings.javascript_access_clipboard));

	browser_settings.javascript_dom_paste =
			getConfig(config, "javascript_dom_paste", STATE_DISABLED);
	GDCEF_DEBUG("Using javascript_dom_paste: "
			<< int(browser_settings.javascript_dom_paste));

	browser_settings.image_loading =
			getConfig(config, "image_loading", STATE_ENABLED);
	GDCEF_DEBUG("Using image loading: " << int(browser_settings.image_loading));

	browser_settings.webgl = getConfig(config, "webgl", STATE_ENABLED);
	GDCEF_DEBUG("Using webgl: " << int(browser_settings.webgl));
}

GDCef::~GDCef() {
	if (m_impl != nullptr) {
		shutdown();
	}
}

GDBrowserView *GDCef::createBrowser(String const &url,
		TextureRect *texture_rect,
		Dictionary config) {
	if (m_impl == nullptr) {
		GDCEF_ERROR("CEF was not created (memory allocation issue)");
		return nullptr;
	}
	if (texture_rect == nullptr) {
		GDCEF_ERROR("You have passed a nullptr texture rectangle");
		return nullptr;
	}

	GDBrowserView *browser = memnew(GDBrowserView());

	CefBrowserSettings settings;
	configureBrowser(settings, config);
	int id = browser->init(url, settings, windowInfo());
	if (id < 0) {
		GDCEF_ERROR("browser->init() failed");
		return nullptr;
	}

	browser->allowDownloads(getConfig(config, "allow_downloads", true));
	browser->setDownloadFolder(
			getConfig(config, "download_folder", String("user://")));

	browser->enableAdBlock(m_browsers_settings.enable_ad_block);
	if (m_browsers_settings.enable_ad_block) {
		for (int i = 0; i < m_browsers_settings.custom_patterns.size(); i++) {
			String pattern = m_browsers_settings.custom_patterns[i];
			browser->addAdBlockPattern(pattern.utf8().get_data());
		}
	}

	browser->resize(texture_rect->get_size());
	texture_rect->set_texture(browser->getTexture());

	add_child(browser);

	String name = browser->get_name();
	GDCEF_DEBUG("name: " << name.utf8().get_data()
						 << ", url: " << url.utf8().get_data());
	return browser;
}

void GDCef::Impl::OnAfterCreated(CefRefPtr<CefBrowser>) {
	CEF_REQUIRE_UI_THREAD();
	GDCEF_DEBUG("");
}

bool GDCef::Impl::DoClose(CefRefPtr<CefBrowser>) {
	CEF_REQUIRE_UI_THREAD();
	GDCEF_DEBUG("");
	return false;
}

void GDCef::Impl::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	GDCEF_DEBUG("");

	int64_t i = m_owner.get_child_count();
	while (i--) {
		Node *node = m_owner.get_child(i);
		GDBrowserView *bv = Object::cast_to<GDBrowserView>(node);
		if ((bv != nullptr) && (bv->id() == browser->GetIdentifier())) {
			GDCEF_DEBUG("Removing browser ID " << bv->id());
			bv->close();
		}
		m_owner.remove_child(node);
		node->queue_free();
	}
}

void GDCef::Impl::closeAllBrowsers(bool force_close) {
	if (!CefCurrentlyOn(TID_UI)) {
		CefPostTask(
				TID_UI,
				base::BindOnce(&GDCef::Impl::closeAllBrowsers, this, force_close));
		return;
	}

	int64_t i = m_owner.get_child_count();
	GDCEF_DEBUG("Removing " << i << " browsers as Godot child nodes");
	while (i--) {
		Node *node = m_owner.get_child(i);
		GDBrowserView *browser = Object::cast_to<GDBrowserView>(node);
		if (browser != nullptr) {
			GDCEF_DEBUG("Removing browser ID " << browser->id());
			browser->close();
		}
		m_owner.remove_child(node);
		node->queue_free();
	}

	GDCEF_DEBUG("Remaining " << m_owner.get_child_count() << " browser nodes");
}
