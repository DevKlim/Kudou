#include "gdcef.hpp"
#include "gdbrowser.hpp"

#include "core/io/dir_access.h"
#include "core/os/os.h"
#include "include/cef_app.h"
#include "servers/display_server.h"

#ifdef WINDOWS_ENABLED
#include "platform/windows/os_windows.h"
#include <windows.h>
#endif

static bool g_cef_initialized = false;

void GDCef::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "config"), &GDCef::initialize);
	ClassDB::bind_method(D_METHOD("is_alive"), &GDCef::isAlive);
	ClassDB::bind_method(D_METHOD("create_browser", "url", "texture_rect", "config"), &GDCef::createBrowser);
	ClassDB::bind_method(D_METHOD("shutdown"), &GDCef::shutdown);
	ClassDB::bind_method(D_METHOD("log_info", "message"), &GDCef::log_info);
	ClassDB::bind_method(D_METHOD("log_warning", "message"), &GDCef::log_warning);
	ClassDB::bind_method(D_METHOD("log_error", "message"), &GDCef::log_error);
	ClassDB::bind_method(D_METHOD("log_fatal", "message"), &GDCef::log_fatal);
}

void GDCef::_init() {
	log_info("GDCef node initialized.");
}

void GDCef::_exit_tree() {
	shutdown();
}

bool GDCef::initialize(Dictionary config) {
	log_info("GDCef::initialize called.");

	if (g_cef_initialized) {
		log_warning("CEF is already initialized.");
		return true;
	}

	m_impl = new GDCef::Impl(*this);

	// 1. Get base paths using the engine executable as an anchor
	String executable_dir = OS::get_singleton()->get_executable_path().get_base_dir();
	log_info(vformat("Executable directory: %s", executable_dir));

	// The engine root is assumed to be one level above the executable directory (e.g., 'bin')
	String engine_root = executable_dir.get_base_dir();
	log_info(vformat("Assumed engine root: %s", engine_root));

	// Path to CEF resource files (.pak, .dat, locales)
	String cef_resources_path = engine_root.path_join("thirdparty/cef_binary");
	log_info(vformat("CEF resources path: %s", cef_resources_path));

	// Path to the browser subprocess executable.
	// It's expected to be alongside the main engine executable.
	String subprocess_path = executable_dir.path_join("gdCefRenderProcess.windows.editor.x86_64.exe");
	log_info(vformat("Looking for subprocess at: %s", subprocess_path));

	// 2. Verify paths exist
	Ref<DirAccess> dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
	if (!dir_access->dir_exists(cef_resources_path)) {
		log_error(vformat("CEF resources path does not exist: %s", cef_resources_path));
		m_impl = nullptr;
		return false;
	}

	if (!dir_access->file_exists(subprocess_path)) {
		log_error(vformat("Browser subprocess executable not found at: %s", subprocess_path));
		m_impl = nullptr;
		return false;
	}
	log_info(vformat("Found browser subprocess at: %s", subprocess_path));

	// 3. Configure CEF settings
	log_info("Configuring CEF settings...");
	CefString(&m_cef_settings.browser_subprocess_path).FromString(subprocess_path.utf8().get_data());

	CefString(&m_cef_settings.resources_dir_path).FromString(cef_resources_path.utf8().get_data());
	log_info(vformat("Setting resources_dir_path: %s", cef_resources_path));

	String locales_path = cef_resources_path.path_join("locales");
	CefString(&m_cef_settings.locales_dir_path).FromString(locales_path.utf8().get_data());
	log_info(vformat("Setting locales_dir_path: %s", locales_path));

	m_cef_settings.no_sandbox = true;
	m_cef_settings.windowless_rendering_enabled = true;

	// Enable verbose logging
	m_cef_settings.log_severity = LOGSEVERITY_VERBOSE;
	String log_file_path = ProjectSettings::get_singleton()->globalize_path("user://cef_debug.log");
	CefString(&m_cef_settings.log_file).FromString(log_file_path.utf8().get_data());
	log_info(vformat("CEF log file path: %s", log_file_path));

	log_info("CEF settings configured.");

	// 4. Configure window info
#ifdef WINDOWS_ENABLED
	log_info("Getting native window handle...");
	HWND hwnd = (HWND)DisplayServer::get_singleton()->window_get_native_handle(DisplayServer::WINDOW_HANDLE, DisplayServer::MAIN_WINDOW_ID);
	log_info(vformat("Native window handle (HWND): 0x%X", (uint64_t)hwnd));
	if (!hwnd) {
		log_error("Failed to get native window handle. Is the main window created?");
		m_impl = nullptr;
		return false;
	}
	m_window_info.SetAsWindowless(hwnd);
#else
	m_window_info.SetAsWindowless(0); // Placeholder for other platforms
#endif

	// 5. Prepare CefMainArgs
	log_info("Preparing CefMainArgs...");
#ifdef WINDOWS_ENABLED
	log_info("Getting native window instance for Windows via GetModuleHandle.");
	HINSTANCE hInstance = GetModuleHandle(NULL);
	log_info(vformat("Application instance (HINSTANCE): 0x%X", (uint64_t)hInstance));
	CefMainArgs main_args(hInstance);
#else
	log_info("Using default CefMainArgs for non-Windows platform.");
	CefMainArgs main_args;
#endif

	// 6. Initialize CEF
	log_info("Calling CefInitialize...");
	if (!CefInitialize(main_args, m_cef_settings, m_impl.get(), nullptr)) {
		log_error("CefInitialize failed. Check for missing CEF resources or architecture mismatch.");
		m_impl = nullptr;
		return false;
	}
	log_info("CefInitialize call completed successfully.");

	g_cef_initialized = true;
	log_info("CEF initialized successfully.");
	return true;
}

bool GDCef::isAlive() {
	return g_cef_initialized && m_impl != nullptr;
}

String GDCef::getError() {
	return String(m_error.str().c_str());
}

void GDCef::log_info(String message) {
	print_line(vformat("[gdCEF] [INFO] %s", message));
}

void GDCef::log_warning(String message) {
	print_line(vformat("[gdCEF] [WARN] %s", message));
}

void GDCef::log_error(String message) {
	print_error(vformat("[gdCEF] [ERROR] %s", message));
}

void GDCef::log_fatal(String message) {
	print_error(vformat("[gdCEF] [FATAL] %s", message));
}

void GDCef::_process(double delta) {
	if (isAlive()) {
		CefDoMessageLoopWork();
	}
}

GDCef::~GDCef() {
	shutdown();
}

void GDCef::shutdown() {
	if (g_cef_initialized) {
		log_info("Shutting down CEF.");
		CefShutdown();
		g_cef_initialized = false;
	}
	m_impl = nullptr;
}

String GDCef::version() {
	return vformat("%d.%d.%d", CEF_VERSION_MAJOR, CEF_VERSION_MINOR, CEF_VERSION_PATCH);
}

int GDCef::versionPart(int entry) {
	switch (entry) {
		case 0:
			return CEF_VERSION_MAJOR;
		case 1:
			return CHROME_VERSION_MAJOR;
		case 2:
			return CHROME_VERSION_MINOR;
		case 3:
			return CHROME_VERSION_BUILD;
		case 4:
			return CHROME_VERSION_PATCH;
		default:
			return 0;
	}
}

GDBrowserView *GDCef::createBrowser(const String &url, TextureRect *texture_rect, Dictionary config) {
	log_info("Creating browser view node.");
	GDBrowserView *view = memnew(GDBrowserView);
	if (!view) {
		log_error("Failed to allocate memory for GDBrowserView.");
		return nullptr;
	}

	add_child(view);

	CefBrowserSettings browser_settings;

	log_info(vformat("Initializing browser view with URL: %s", url));
	if (view->init(url, browser_settings, m_window_info) == -1) {
		log_error("Failed to initialize GDBrowserView (init returned -1).");
		view->queue_free();
		return nullptr;
	}

	if (texture_rect) {
		log_info("Setting texture on the provided TextureRect.");
		texture_rect->set_texture(view->getTexture());
	} else {
		log_warning("No TextureRect provided to createBrowser.");
	}

	log_info("Browser view created and initialized successfully.");
	return view;
}

void GDCef::Impl::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	m_owner.log_info(vformat("Browser created with ID: %d", browser->GetIdentifier()));
}

bool GDCef::Impl::DoClose(CefRefPtr<CefBrowser> browser) {
	return false;
}

void GDCef::Impl::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	m_owner.log_info(vformat("Browser with ID %d is closing.", browser->GetIdentifier()));
}

void GDCef::Impl::closeAllBrowsers(bool force_close) {
	// This is handled by CEF shutdown
}