#include "gdbrowser.hpp"
#include "helper_config.hpp"
#include "helper_files.hpp"

#include "core/error/error_list.h"
#include "core/io/file_access.h" // Added for FileAccess::exists
#include "core/io/json.h"
#include "core/object/class_db.h"

// Add guards for CEF conflicts after Godot headers
// Guards for CEF conflicts are now in gdbrowser.hpp (via ad_blocker.hpp)

// Now include CEF headers
#include "include/base/cef_logging.h"

#ifdef _OPENMP
#include <omp.h>
#define OPENMP_PARALLEL_FOR #pragma omp parallel for
#define PARALLEL_FOR OPENMP_PARALLEL_FOR for
#else
#define PARALLEL_FOR for
#endif

class Visitor : public CefStringVisitor {
public:
	Visitor(GDBrowserView &node) :
			m_node(node) {}

	virtual void Visit(const CefString &string) override {
		String html(string.ToString().c_str());

		m_node.emit_signal("on_html_content_requested", html, &m_node);
	}

private:
	GDBrowserView &m_node;
	IMPLEMENT_REFCOUNTING(Visitor);
};

GDBrowserView::Impl::~Impl() {
	WARN_PRINT("[gdCEF][GDBrowserView::Impl::~Impl] destroying browser");
}

void GDBrowserView::_bind_methods() {
	WARN_PRINT("[gdCEF][GDBrowserView::_bind_methods]");

	ClassDB::bind_method(D_METHOD("close"), &GDBrowserView::close);
	ClassDB::bind_method(D_METHOD("id"), &GDBrowserView::id);
	ClassDB::bind_method(D_METHOD("get_error"), &GDBrowserView::getError);
	ClassDB::bind_method(D_METHOD("is_valid"), &GDBrowserView::isValid);
	ClassDB::bind_method(D_METHOD("set_texture", "texture"),
			&GDBrowserView::setTexture);
	ClassDB::bind_method(D_METHOD("get_texture"), &GDBrowserView::getTexture);
	ClassDB::bind_method(D_METHOD("set_zoom_level"),
			&GDBrowserView::setZoomLevel);
	ClassDB::bind_method(D_METHOD("get_title"), &GDBrowserView::getTitle);
	ClassDB::bind_method(D_METHOD("get_url"), &GDBrowserView::getURL);
	ClassDB::bind_method(D_METHOD("load_url"), &GDBrowserView::loadURL);
	ClassDB::bind_method(D_METHOD("load_data_uri"),
			&GDBrowserView::loadDataURI);
	ClassDB::bind_method(D_METHOD("download_file"),
			&GDBrowserView::downloadFile);
	ClassDB::bind_method(D_METHOD("allow_downloads"),
			&GDBrowserView::allowDownloads);
	ClassDB::bind_method(D_METHOD("set_download_folder"),
			&GDBrowserView::setDownloadFolder);
	ClassDB::bind_method(D_METHOD("is_loaded"), &GDBrowserView::loaded);
	ClassDB::bind_method(D_METHOD("reload"), &GDBrowserView::reload);
	ClassDB::bind_method(D_METHOD("stop_loading"), &GDBrowserView::stopLoading);
	ClassDB::bind_method(D_METHOD("copy"), &GDBrowserView::copy);
	ClassDB::bind_method(D_METHOD("paste"), &GDBrowserView::paste);
	ClassDB::bind_method(D_METHOD("undo"), &GDBrowserView::undo);
	ClassDB::bind_method(D_METHOD("redo"), &GDBrowserView::redo);
	ClassDB::bind_method(D_METHOD("request_html_content"),
			&GDBrowserView::requestHtmlContent);
	ClassDB::bind_method(D_METHOD("has_previous_page"),
			&GDBrowserView::canNavigateBackward);
	ClassDB::bind_method(D_METHOD("has_next_page"),
			&GDBrowserView::canNavigateForward);
	ClassDB::bind_method(D_METHOD("previous_page"),
			&GDBrowserView::navigateBackward);
	ClassDB::bind_method(D_METHOD("next_page"),
			&GDBrowserView::navigateForward);
	ClassDB::bind_method(D_METHOD("resize"), &GDBrowserView::resize_);
	ClassDB::bind_method(D_METHOD("set_viewport"), &GDBrowserView::viewport);
	ClassDB::bind_method(D_METHOD("set_key_pressed"), &GDBrowserView::keyPress);
	ClassDB::bind_method(D_METHOD("set_mouse_moved"),
			&GDBrowserView::mouseMove);
	ClassDB::bind_method(D_METHOD("set_mouse_left_click"),
			&GDBrowserView::leftClick);
	ClassDB::bind_method(D_METHOD("set_mouse_right_click"),
			&GDBrowserView::rightClick);
	ClassDB::bind_method(D_METHOD("set_mouse_middle_click"),
			&GDBrowserView::middleClick);
	ClassDB::bind_method(D_METHOD("set_mouse_left_down"),
			&GDBrowserView::leftMouseDown);
	ClassDB::bind_method(D_METHOD("set_mouse_left_up"),
			&GDBrowserView::leftMouseUp);
	ClassDB::bind_method(D_METHOD("set_mouse_right_down"),
			&GDBrowserView::rightMouseDown);
	ClassDB::bind_method(D_METHOD("set_mouse_right_up"),
			&GDBrowserView::rightMouseUp);
	ClassDB::bind_method(D_METHOD("set_mouse_middle_down"),
			&GDBrowserView::middleMouseDown);
	ClassDB::bind_method(D_METHOD("set_mouse_middle_up"),
			&GDBrowserView::middleMouseUp);
	ClassDB::bind_method(D_METHOD("set_mouse_wheel_vertical"),
			&GDBrowserView::mouseWheelVertical);
	ClassDB::bind_method(D_METHOD("set_mouse_wheel_horizontal"),
			&GDBrowserView::mouseWheelHorizontal);
	ClassDB::bind_method(D_METHOD("set_muted"), &GDBrowserView::mute);
	ClassDB::bind_method(D_METHOD("is_muted"), &GDBrowserView::muted);
	ClassDB::bind_method(D_METHOD("set_audio_stream", "audio"),
			&GDBrowserView::setAudioStreamer);
	ClassDB::bind_method(D_METHOD("get_audio_stream"),
			&GDBrowserView::getAudioStreamer);
	ClassDB::bind_method(D_METHOD("get_pixel_color", "x", "y"),
			&GDBrowserView::getPixelColor);
	ClassDB::bind_method(D_METHOD("register_method", "object", "method"),
			&GDBrowserView::registerGodotMethod);
	ClassDB::bind_method(D_METHOD("js_emit", "event_name", "data"),
			&GDBrowserView::jsEmit);
	ClassDB::bind_method(D_METHOD("execute_javascript"),
			&GDBrowserView::executeJavaScript);
	ClassDB::bind_method(D_METHOD("add_ad_block_pattern", "pattern"),
			&GDBrowserView::addAdBlockPattern);
	ClassDB::bind_method(D_METHOD("enable_ad_block", "enable"),
			&GDBrowserView::enableAdBlock);
	ClassDB::bind_method(D_METHOD("is_ad_block_enabled"),
			&GDBrowserView::isAdBlockEnabled);
	ClassDB::bind_method(D_METHOD("log_info", "message"), &GDBrowserView::log_info);
	ClassDB::bind_method(D_METHOD("log_warning", "message"), &GDBrowserView::log_warning);
	ClassDB::bind_method(D_METHOD("log_error", "message"), &GDBrowserView::log_error);
	ClassDB::bind_method(D_METHOD("log_fatal", "message"), &GDBrowserView::log_fatal);

	ADD_SIGNAL(MethodInfo("on_download_updated",
			PropertyInfo(Variant::STRING, "file"),
			PropertyInfo(Variant::INT, "percentage"),
			PropertyInfo(Variant::OBJECT, "browser")));
	ADD_SIGNAL(
			MethodInfo("on_page_loaded", PropertyInfo(Variant::OBJECT, "browser")));
	ADD_SIGNAL(MethodInfo("on_page_failed_loading",
			PropertyInfo(Variant::INT, "err_code"),
			PropertyInfo(Variant::STRING, "err_msg"),
			PropertyInfo(Variant::OBJECT, "browser")));
	ADD_SIGNAL(MethodInfo("on_browser_paint",
			PropertyInfo(Variant::OBJECT, "browser")));
	ADD_SIGNAL(MethodInfo("on_html_content_requested",
			PropertyInfo(Variant::STRING, "html"),
			PropertyInfo(Variant::OBJECT, "browser")));

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT,
						 "audio_stream",
						 PROPERTY_HINT_NODE_TYPE,
						 "AudioStreamGeneratorPlayback"),
			"set_audio_stream",
			"get_audio_stream");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT,
						 "texture",
						 PROPERTY_HINT_NODE_TYPE,
						 "ImageTexture"),
			"set_texture",
			"get_texture");
}

void GDBrowserView::_init() {
	BROWSER_DEBUG("");
}

String GDBrowserView::getError() {
	std::string err = m_error.str();
	m_error.clear();
	return String(err.c_str());
}

int GDBrowserView::init(const String &url,
		CefBrowserSettings const &settings,
		CefWindowInfo const &window_info) {
	if (m_impl == nullptr) {
		GDCEF_ERROR("GDBrowserView::init: m_impl is nullptr. Browser initialization failed.");
		return -1;
	}

	BROWSER_DEBUG("Attempting to create browser with URL: " << url.utf8().get_data());

	m_browser = CefBrowserHost::CreateBrowserSync(
			window_info, m_impl, url.utf8().get_data(), settings, nullptr, nullptr);

	if ((m_browser == nullptr) || (m_browser->GetHost() == nullptr)) {
		m_id = -1;
		BROWSER_ERROR("CreateBrowserSync failed");
	} else {
		std::string name("browser_");
		name += std::to_string(m_browser->GetIdentifier());
		set_name(name.c_str());

		m_id = m_browser->GetIdentifier();
		m_browser->GetHost()->WasResized();
	}

	return m_id;
}

GDBrowserView::GDBrowserView() :
		m_viewport({ 0.0f, 0.0f, 1.0f, 1.0f }) {
	BROWSER_DEBUG("Creating new GDBrowserView");

	m_impl = new GDBrowserView::Impl(*this);
	assert((m_impl != nullptr) && "Failed allocating GDBrowserView");
	m_image.instantiate();
	m_texture.instantiate();
}

GDBrowserView::~GDBrowserView() {
	close();
}

void GDBrowserView::getViewRect(CefRefPtr<CefBrowser>,
		CefRect &rect) {
	rect = CefRect(int(m_viewport[0] * m_width),
			int(m_viewport[1] * m_height),
			int(m_viewport[2] * m_width),
			int(m_viewport[3] * m_height));
}

//------------------------------------------------------------------------------
void GDBrowserView::onPaint(CefRefPtr<CefBrowser> /*browser*/,
		CefRenderHandler::PaintElementType /*type*/,
		const CefRenderHandler::RectList &dirtyRects,
		const void *buffer,
		int width,
		int height) {
	if ((width <= 0) || (height <= 0) || (buffer == nullptr)) {
		return;
	}

	int const COLOR_CHANELS = 4;
	int const SIZEOF_COLOR = COLOR_CHANELS * sizeof(char);
	int const TEXTURE_SIZE = SIZEOF_COLOR * width * height;

	bool bResized = m_p_data.size() != TEXTURE_SIZE;

	m_p_data.resize(TEXTURE_SIZE);

	unsigned char *imageData = m_p_data.ptrw();
	const unsigned char *cbuffer = (const unsigned char *)buffer;
	auto doCopyLine = [imageData, cbuffer, width, COLOR_CHANELS](
							  int line, int x, int copyWidth) {
		int i = (line * width + x) * COLOR_CHANELS;
		int end = i + (copyWidth * COLOR_CHANELS);
		for (; i < end; i += COLOR_CHANELS) {
			imageData[i + 0] = cbuffer[i + 2];
			imageData[i + 1] = cbuffer[i + 1];
			imageData[i + 2] = cbuffer[i + 0];
			imageData[i + 3] = cbuffer[i + 3];
		}
	};

	if (bResized) {
		PARALLEL_FOR(int y = 0; y < height; ++y) {
			doCopyLine(y, 0, width);
		}

		m_image->set_data(
				width, height, false, Image::FORMAT_RGBA8, m_p_data);
		m_texture->set_image(m_image);
	} else {
		for (const CefRect &rect : dirtyRects) {
			PARALLEL_FOR(int y = rect.y; y < rect.y + rect.height; ++y) {
				doCopyLine(y, rect.x, rect.width);
			}
		}

		m_image->set_data(
				width, height, false, Image::FORMAT_RGBA8, m_p_data);
		m_texture->update(m_image);
	}

	emit_signal("on_browser_paint", this);
}

void GDBrowserView::onLoadEnd(CefRefPtr<CefBrowser>,
		CefRefPtr<CefFrame> frame,
		int httpStatusCode) {
	if ((httpStatusCode == 200) && (frame->IsMain())) {
		BROWSER_DEBUG("has ended loading " << frame->GetURL());
		emit_signal("on_page_loaded", this);
	}
}

void GDBrowserView::onLoadError(CefRefPtr<CefBrowser>,
		CefRefPtr<CefFrame> frame,
		const int errCode,
		const CefString &errorText) {
	CEF_REQUIRE_UI_THREAD();

	if (errCode == ERR_ABORTED) {
		return;
	}

	if (frame->IsMain()) {
		std::string str = errorText.ToString();
		BROWSER_ERROR("has failed loading " << frame->GetURL() << ": " << str);
		String msg(str.c_str());
		emit_signal("on_page_failed_loading", errCode, msg, this);
	}
}

void GDBrowserView::setZoomLevel(double delta) {
	BROWSER_DEBUG(delta);

	if (!m_browser) {
		return;
	}

	m_browser->GetHost()->SetZoomLevel(delta);
}

//------------------------------------------------------------------------------
void GDBrowserView::loadURL(String url) {
	// Not a Godot path
	if (!url.begins_with("res://") && !url.begins_with("user://")) {
		m_browser->GetMainFrame()->LoadURL(url.utf8().get_data());
		return;
	}

	// Convert Godot path to system path
	String local_path = ProjectSettings::get_singleton()->globalize_path(url);

	// Check if the file exists
	if (!FileAccess::exists(local_path)) {
		BROWSER_ERROR("File not found: " << local_path.utf8().get_data());
		// Create error HTML page
		std::string error_html("<html><body bgcolor=\"white\">");
		error_html += "<h2>File not found: ";
		error_html += url.utf8().get_data();
		error_html += "</h2></body></html>";
		loadDataURI(String(error_html.c_str()), "text/html");
		return;
	}

	// Build the file:// URL
	String file_url = "file://" + local_path;
	BROWSER_DEBUG(file_url.utf8().get_data());
	m_browser->GetMainFrame()->LoadURL(file_url.utf8().get_data());
}

void GDBrowserView::loadDataURI(String html, String mime_type) {
	auto const &d = html.utf8();
	std::string uri("data:");
	uri += mime_type.utf8().get_data();
	uri += ";base64,";
	uri += CefURIEncode(CefBase64Encode(d.ptr(), d.length()), false).ToString();
	m_browser->GetMainFrame()->LoadURL(uri);
}

bool GDBrowserView::reload() const {
	BROWSER_DEBUG("");

	if (!m_browser) {
		return false;
	}

	m_browser->Reload();
	return true;
}

bool GDBrowserView::loaded() const {
	BROWSER_DEBUG("");

	if (!m_browser) {
		return false;
	}

	return m_browser->HasDocument();
}

String GDBrowserView::getURL() const {
	if (m_browser && m_browser->GetMainFrame()) {
		std::string str = m_browser->GetMainFrame()->GetURL().ToString();
		BROWSER_DEBUG(str);
		return String(str.c_str());
	}

	BROWSER_ERROR("Not possible to retrieving URL");
	return String();
}

String GDBrowserView::getTitle() const {
	BROWSER_DEBUG("");

	if (!m_browser) {
		return String();
	}

	if (m_browser->GetMainFrame()) {
		CefString title =
				m_browser->GetHost()->GetVisibleNavigationEntry()->GetTitle();
		std::string utf8_title = title.ToString();
		return String::utf8(utf8_title.c_str());
	}

	return String();
}

void GDBrowserView::stopLoading() {
	BROWSER_DEBUG("");

	if (!m_browser) {
		return;
	}

	m_browser->StopLoad();
}

void GDBrowserView::copy() const {
	BROWSER_DEBUG("");

	if (m_browser && m_browser->GetMainFrame()) {
		m_browser->GetMainFrame()->Copy();
	} else {
		BROWSER_ERROR("copy failed");
	}
}

void GDBrowserView::paste() const {
	BROWSER_DEBUG("");

	if (m_browser && m_browser->GetMainFrame()) {
		m_browser->GetMainFrame()->Paste();
	} else {
		BROWSER_ERROR("paste failed");
	}
}

void GDBrowserView::cut() const {
	BROWSER_DEBUG("");

	if (m_browser && m_browser->GetMainFrame()) {
		m_browser->GetMainFrame()->Cut();
	} else {
		BROWSER_ERROR("cut failed");
	}
}

void GDBrowserView::delete_() const {
	BROWSER_DEBUG("");

	if (m_browser && m_browser->GetMainFrame()) {
		m_browser->GetMainFrame()->Delete();
	} else {
		BROWSER_ERROR("delete failed");
	}
}

void GDBrowserView::undo() const {
	BROWSER_DEBUG("");

	if (m_browser && m_browser->GetMainFrame()) {
		m_browser->GetMainFrame()->Undo();
	} else {
		BROWSER_ERROR("undo failed");
	}
}

void GDBrowserView::redo() const {
	BROWSER_DEBUG("");

	if (m_browser && m_browser->GetMainFrame()) {
		m_browser->GetMainFrame()->Redo();
	} else {
		BROWSER_ERROR("redo failed");
	}
}

void GDBrowserView::requestHtmlContent() {
	CefRefPtr<Visitor> visitor = new Visitor(*this);
	if (m_browser && m_browser->GetMainFrame()) {
		m_browser->GetMainFrame()->GetSource(visitor);
	}

	BROWSER_ERROR("Not possible to retrieving text");
}

void GDBrowserView::executeJavaScript(String javascript) {
	BROWSER_DEBUG("");

	if (m_browser && m_browser->GetMainFrame()) {
		CefString codeStr;
		codeStr.FromString(javascript.utf8().get_data());
		CefString urlStr;
		m_browser->GetMainFrame()->ExecuteJavaScript(codeStr, urlStr, 0);
	} else {
		BROWSER_ERROR("executeJavaScript failed");
	}
}

bool GDBrowserView::canNavigateBackward() const {
	BROWSER_DEBUG("");

	if (!m_browser) {
		return false;
	}

	return m_browser->CanGoBack();
}

void GDBrowserView::navigateBackward() {
	BROWSER_DEBUG("");

	if ((m_browser != nullptr) && (m_browser->CanGoBack())) {
		m_browser->GoBack();
	}
}

bool GDBrowserView::canNavigateForward() const {
	BROWSER_DEBUG("");

	if (!m_browser) {
		return false;
	}

	return m_browser->CanGoForward();
}

void GDBrowserView::navigateForward() {
	BROWSER_DEBUG("");

	if ((m_browser != nullptr) && (m_browser->CanGoForward())) {
		m_browser->GoForward();
	}
}

void GDBrowserView::resize_(int width, int height) {
	if (width <= 0) {
		width = 2;
	}
	if (height <= 0) {
		height = 2;
	}
	BROWSER_DEBUG(width << " x " << height);

	m_width = float(width);
	m_height = float(height);

	if (!m_browser || !m_browser->GetHost()) {
		return;
	}

	m_browser->GetHost()->WasResized();
}

bool GDBrowserView::viewport(float x, float y, float w, float h) {
	if (!(x >= 0.0f) && (x < 1.0f)) {
		return false;
	}

	if (!(x >= 0.0f) && (y < 1.0f)) {
		return false;
	}

	if (!(w > 0.0f) && (w <= 1.0f)) {
		return false;
	}

	if (!(h > 0.0f) && (h <= 1.0f)) {
		return false;
	}

	if (x + w > 1.0f) {
		return false;
	}

	if (y + h > 1.0f) {
		return false;
	}

	m_viewport[0] = x;
	m_viewport[1] = y;
	m_viewport[2] = w;
	m_viewport[3] = h;

	return true;
}

bool GDBrowserView::isValid() const {
	BROWSER_DEBUG("");

	if (!m_browser) {
		return false;
	}

	return m_browser->IsValid();
}

void GDBrowserView::close() {
	if (!m_browser) {
		return;
	}

	String name = get_name();
	BROWSER_DEBUG("Closing browser " << m_id << " '" << name.utf8().get_data()
									 << "'");

	auto host = m_browser->GetHost();
	if (!host) {
		return;
	}

	host->CloseDevTools();
	host->CloseBrowser(true);
	m_browser = nullptr;
	m_impl = nullptr;
}

bool GDBrowserView::mute(bool mute) {
	CEF_REQUIRE_UI_THREAD();
	if (m_browser == nullptr) {
		return true;
	}

	m_browser->GetHost()->SetAudioMuted(mute);
	return m_browser->GetHost()->IsAudioMuted();
}

bool GDBrowserView::muted() {
	CEF_REQUIRE_UI_THREAD();
	if (m_browser == nullptr) {
		return true;
	}

	return m_browser->GetHost()->IsAudioMuted();
}

void GDBrowserView::onAudioStreamStarted(CefRefPtr<CefBrowser> browser,
		const CefAudioParameters &params,
		int channels) {
	m_impl->m_audio.channels = int(params.channel_layout);
}

//------------------------------------------------------------------------------
void GDBrowserView::onAudioStreamPacket(CefRefPtr<CefBrowser> browser,
		const float **p_data,
		int frames,
		int64_t pts) {
	if ((m_impl == nullptr) || (m_impl->m_audio.streamer == nullptr)) {
		return;
	}

	if ((p_data == nullptr) || (frames <= 0) || (m_impl->m_audio.channels == -1)) {
		return;
	}

	Ref<AudioStreamGeneratorPlayback> streamer = m_impl->m_audio.streamer;
	if (streamer.is_valid() && streamer->can_push_buffer(frames)) {
		for (int i = 0; i < frames; i++) {
			streamer->push_frame(Vector2(p_data[0][i], p_data[0][i]));
		}
	}
}

//------------------------------------------------------------------------------
Color GDBrowserView::getPixelColor(int x, int y) const {
	if (x < 0 || y < 0 || x >= m_width || y >= m_height || m_p_data.size() == 0) {
		return Color(1, 1, 1, 1);
	}

	int index = (y * m_width + x) * 4;
	unsigned char r = m_p_data[index + 0];
	unsigned char g = m_p_data[index + 1];
	unsigned char b = m_p_data[index + 2];
	unsigned char a = m_p_data[index + 3];

	return Color(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
}

bool GDBrowserView::onBeforePopup(CefRefPtr<CefBrowser> browser,
		const CefString &target_url) {
	browser->GetMainFrame()->LoadURL(target_url);
	return true;
}

void GDBrowserView::allowDownloads(bool allow) {
	m_allow_downloads = allow;
}

void GDBrowserView::setDownloadFolder(String path) {
	if (path.begins_with("user://") || path.begins_with("res://")) {
		// Convert Godot String to std::string before assigning to fs::path
		m_download_folder = ProjectSettings::get_singleton()->globalize_path(path).utf8().get_data();
	} else {
		m_download_folder = path.utf8().get_data();
	}
}

void GDBrowserView::downloadFile(String url) {
	m_browser->GetHost()->StartDownload(url.utf8().get_data());
}

bool GDBrowserView::canDownload(CefRefPtr<CefBrowser> browser,
		const CefString &url,
		const CefString &request_method) {
	return m_allow_downloads;
}

bool GDBrowserView::onBeforeDownload(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDownloadItem> download_item,
		const CefString &suggested_name,
		CefRefPtr<CefBeforeDownloadCallback> callback) {
	fs::path download_path =
			fs::path(m_download_folder) / fs::path(suggested_name.c_str());
	BROWSER_DEBUG("Downloading file for path " << download_path.string());

	callback->Continue(download_path.string().c_str(), false);

	return false;
}

void GDBrowserView::onDownloadUpdated(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDownloadItem> download_item,
		CefRefPtr<CefDownloadItemCallback> callback) {
	int percentage = download_item->GetPercentComplete();
	std::string file = download_item->GetFullPath();

	BROWSER_DEBUG("Download " << file << " Updated: " << percentage);

	if ((percentage == 100) && (download_item->IsComplete())) {
		BROWSER_DEBUG("Download " << file << " Complete");
	}

	emit_signal(
			"on_download_updated", String(file.c_str()), percentage, this);
}

bool GDBrowserView::registerGodotMethod(Object *object,
		String method_name) {
	BROWSER_DEBUG("Registering gdscript method "
			<< method_name.utf8().get_data());

	Callable callable(object, method_name);
	if (!callable.is_valid()) {
		BROWSER_ERROR("Invalid callable gdscript method "
				<< method_name.utf8().get_data());
		return false;
	}

	if (method_name.begins_with("_")) {
		m_js_bindings[method_name.substr(1).utf8().get_data()] = callable;
	} else {
		m_js_bindings[method_name.utf8().get_data()] = callable;
	}

	return true;
}

Variant GDBrowserView::JsonToGodot(const Dictionary &json) {
	if (json.has("type") && json["type"] == "binary" && json.has("format") &&
			json["format"] == "base64" && json.has("data") && json.has("size")) {
		String base64_data = json["data"];
		int expected_size = json["size"];

		std::string base64_str = base64_data.utf8().get_data();

		CefRefPtr<CefBinaryValue> binary = CefBase64Decode(base64_str);
		if (binary.get() && binary->GetSize() > 0) {
			PackedByteArray byte_array;
			byte_array.resize(binary->GetSize());

			binary->GetData(byte_array.ptrw(), binary->GetSize(), 0);

			if (byte_array.size() != expected_size) {
				BROWSER_DEBUG("Binary size mismatch: expected "
						<< expected_size << " but got "
						<< byte_array.size());
			}

			return byte_array;
		} else {
			BROWSER_ERROR("Failed to decode base64 data");
			return Variant();
		}
	}

	Dictionary result;

	Array keys = json.keys();
	for (int i = 0; i < keys.size(); i++) {
		Variant key = keys[i];
		Variant value = json[key];

		if (value.get_type() == Variant::Type::DICTIONARY) {
			Dictionary dict = value;
			result[key] = JsonToGodot(dict);
		} else if (value.get_type() == Variant::Type::ARRAY) {
			Array array = value;
			result[key] = JsonToGodot(array);
		} else {
			result[key] = value;
		}
	}

	return result;
}

Variant GDBrowserView::JsonToGodot(const Array &json_array) {
	Array result;
	result.resize(json_array.size());

	for (int i = 0; i < json_array.size(); i++) {
		Variant element = json_array[i];

		if (element.get_type() == Variant::Type::DICTIONARY) {
			Dictionary dict = element;
			result[i] = JsonToGodot(dict);
		} else if (element.get_type() == Variant::Type::ARRAY) {
			Array arr = element;
			result[i] = JsonToGodot(arr);
		} else {
			result[i] = element;
		}
	}

	return result;
}

Variant GDBrowserView::JsonToGodot(const Variant &json_value) {
	switch (json_value.get_type()) {
		case Variant::Type::DICTIONARY: {
			Dictionary dict = json_value;
			return JsonToGodot(dict);
		}

		case Variant::Type::ARRAY: {
			Array arr = json_value;
			return JsonToGodot(arr);
		}

		case Variant::Type::NIL:
		case Variant::Type::BOOL:
		case Variant::Type::INT:
		case Variant::Type::FLOAT:
		case Variant::Type::STRING:
		default:
			return json_value;
	}
}

bool GDBrowserView::onProcessMessageReceived(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) {
	BROWSER_DEBUG("Received message " << message->GetName().ToString());

	if (message->GetName() != "callGodotMethod") {
		BROWSER_DEBUG("Expecting IPC command 'callGodotMethod'");
		return false;
	}

	CefRefPtr<CefListValue> args_list = message->GetArgumentList();
	if (args_list->GetSize() < 2) {
		BROWSER_ERROR("Expected method name and JSON arguments for "
					  "'callGodotMethod' IPC command");
		return false;
	}

	std::string method_name = args_list->GetString(0).ToString();

	auto callable = m_js_bindings[method_name];
	if (!callable.is_valid()) {
		BROWSER_ERROR("Callable not found for method " << method_name);
		return false;
	}

	try {
		std::string json_args = args_list->GetString(1).ToString();
		String json_godot(json_args.c_str());

		Variant parsed = JSON::parse_string(json_godot);

		if (parsed.get_type() == Variant::Type::NIL &&
				!json_godot.is_empty() && json_godot != "null") {
			BROWSER_ERROR("Failed to parse JSON arguments: "
					<< json_godot.utf8().get_data());
			return false;
		}

		Array args;

		if (parsed.get_type() == Variant::Type::ARRAY) {
			Array array = parsed;
			args = JsonToGodot(array);
		} else if (parsed.get_type() != Variant::Type::NIL) {
			args.push_back(JsonToGodot(parsed));
		}

		callable.callv(args);
		return true;
	} catch (const std::exception &e) {
		BROWSER_ERROR("Exception in onProcessMessageReceived: " << e.what());
		return false;
	}
}

//------------------------------------------------------------------------------
bool GDBrowserView::jsEmit(String event_name, const Variant &p_data) {
	BROWSER_DEBUG("Sending message to render process '"
			<< event_name.utf8().get_data() << "'");

	if (!m_browser || !m_browser->GetMainFrame()) {
		BROWSER_ERROR("Browser not ready");
		return false;
	}

	try {
		CefRefPtr<CefProcessMessage> message =
				CefProcessMessage::Create("godotEvents.emit");
		CefRefPtr<CefListValue> args = message->GetArgumentList();

		args->SetString(0, event_name.utf8().get_data());

		if (p_data.get_type() == Variant::Type::PACKED_BYTE_ARRAY) {
			PackedByteArray binary_data = p_data;

			std::string base64 =
					CefBase64Encode(binary_data.ptr(), binary_data.size());

			std::string json = "{";
			json += "\"type\":\"binary\",";
			json += "\"format\":\"base64\",";
			json += "\"data\":\"" + base64 + "\",";
			json += "\"size\":" + std::to_string(binary_data.size());
			json += "}";

			args->SetString(1, json);
		} else {
			String json_data = JSON::stringify(p_data);
			args->SetString(1, json_data.utf8().get_data());
		}

		m_browser->GetMainFrame()->SendProcessMessage(PID_RENDERER, message);
		return true;
	} catch (const std::exception &e) {
		BROWSER_ERROR("Error sending message to render process: " << e.what());
		return false;
	}
}

bool GDBrowserView::addAdBlockPattern(String pattern) {
	if (m_impl && m_impl->m_ad_blocker) {
		return m_impl->m_ad_blocker->addPattern(pattern.utf8().get_data());
	}
	BROWSER_ERROR("addAdBlockPattern failed: browser not fully initialized.");
	return false;
}

void GDBrowserView::enableAdBlock(bool enable) {
	if (m_impl && m_impl->m_ad_blocker) {
		m_impl->m_ad_blocker->enable(enable);
	} else {
		BROWSER_ERROR("enableAdBlock failed: browser not fully initialized.");
	}
}

bool GDBrowserView::isAdBlockEnabled() const {
	if (m_impl && m_impl->m_ad_blocker) {
		return m_impl->m_ad_blocker->is_enabled();
	}
	return false;
}

void GDBrowserView::log_info(String message) {
	LOG(INFO) << "[GDBrowserView] " << message.utf8().get_data();
}

void GDBrowserView::log_warning(String message) {
	LOG(WARNING) << "[GDBrowserView] " << message.utf8().get_data();
}

void GDBrowserView::log_error(String message) {
	LOG(ERROR) << "[GDBrowserView] " << message.utf8().get_data();
}

void GDBrowserView::log_fatal(String message) {
	LOG(FATAL) << "[GDBrowserView] " << message.utf8().get_data();
}