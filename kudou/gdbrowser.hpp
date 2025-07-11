#ifndef GDCEF_BROWSER_HPP
#define GDCEF_BROWSER_HPP

#include "helper_files.hpp"

// Godot headers
#include "core/object/class_db.h"
#include "scene/main/node.h"
#include "scene/resources/image_texture.h"
#include "servers/audio/audio_stream.h" // Corrected path for Godot 4.x
#include "servers/audio/effects/audio_stream_generator.h" // Corrected path

#include "cef_precomp.h"

#include <array>
#include <chrono>
#include <iostream> // Kept for potential future debugging, though specific print removed

class GDBrowserView : public Node {
	GDCLASS(GDBrowserView, Node);
	friend class GDCef;

public:
	void _init();

protected:
	static void _bind_methods();

private:
	struct RoutingAudio {
		Ref<AudioStreamGeneratorPlayback> streamer = nullptr;
		PackedVector2Array buffer;
		int channels = -1;
	};

	class Impl : public CefClient,
				 public CefRenderHandler,
				 public CefLoadHandler,
				 public CefAudioHandler,
				 public CefLifeSpanHandler,
				 public CefDownloadHandler,
				 public CefRequestHandler,
				 public CefResourceRequestHandler {
	public:
		friend GDBrowserView;

		Impl(GDBrowserView &view) :
				m_owner(view) {
			m_ad_blocker = new AdBlocker();
			assert((m_ad_blocker != nullptr) && "Failed allocating AdBlocker");
		}

		virtual ~Impl();

	private:
		IMPLEMENT_REFCOUNTING(Impl);

		virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override {
			return this;
		}

		virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override {
			return this;
		}

		virtual CefRefPtr<CefAudioHandler> GetAudioHandler() override {
			// Removed debug print statement
			return m_audio.streamer != nullptr ? this : nullptr;
		}

		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
			return this;
		}

		virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() override {
			return this;
		}

		virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override {
			return this;
		}

		virtual bool
		OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefFrame> frame,
				CefProcessId source_process,
				CefRefPtr<CefProcessMessage> message) override {
			return m_owner.onProcessMessageReceived(
					browser, frame, source_process, message);
		}

	private:
		virtual void GetViewRect(CefRefPtr<CefBrowser> browser,
				CefRect &rect) override {
			m_owner.getViewRect(browser, rect);
		}

		virtual void OnPaint(CefRefPtr<CefBrowser> browser,
				CefRenderHandler::PaintElementType type,
				const CefRenderHandler::RectList &dirtyRects,
				const void *buffer,
				int width,
				int height) override {
			m_owner.onPaint(browser, type, dirtyRects, buffer, width, height);
		}

	private:
		virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefFrame> frame,
				int httpStatusCode) override {
			m_owner.onLoadEnd(browser, frame, httpStatusCode);
		}

		virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefFrame> frame,
				ErrorCode errorCode,
				const CefString &errorText,
				const CefString &failedUrl) override {
			m_owner.onLoadError(browser, frame, int(errorCode), errorText);
		}

	private:
		virtual void OnAudioStreamStarted(CefRefPtr<CefBrowser> browser,
				const CefAudioParameters &params,
				int channels) override {
			m_owner.onAudioStreamStarted(browser, params, channels);
		}

		virtual void OnAudioStreamPacket(CefRefPtr<CefBrowser> browser,
				const float **p_data,
				int frames,
				int64_t pts) override {
			m_owner.onAudioStreamPacket(browser, p_data, frames, pts);
		}

		virtual void
		OnAudioStreamStopped(CefRefPtr<CefBrowser> browser) override {
			// No action needed in this handler
		}

		virtual void OnAudioStreamError(CefRefPtr<CefBrowser> browser,
				const CefString &message) override {
			// No action needed in this handler
		}

	private:
		virtual bool OnBeforePopup(
				CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefFrame> frame,
				int popup_id,
				const CefString &target_url,
				const CefString &target_frame_name,
				CefLifeSpanHandler::WindowOpenDisposition target_disposition,
				bool user_gesture,
				const CefPopupFeatures &popupFeatures,
				CefWindowInfo &windowInfo,
				CefRefPtr<CefClient> &client,
				CefBrowserSettings &settings,
				CefRefPtr<CefDictionaryValue> &extra_info,
				bool *no_javascript_access) override {
			return m_owner.onBeforePopup(browser, target_url);
		}

	private:
		virtual bool CanDownload(CefRefPtr<CefBrowser> browser,
				const CefString &url,
				const CefString &request_method) override {
			return m_owner.canDownload(browser, url, request_method);
		}

		virtual bool
		OnBeforeDownload(CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefDownloadItem> download_item,
				const CefString &suggested_name,
				CefRefPtr<CefBeforeDownloadCallback> callback) override {
			return m_owner.onBeforeDownload(
					browser, download_item, suggested_name, callback);
		}

		virtual void
		OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefDownloadItem> download_item,
				CefRefPtr<CefDownloadItemCallback> callback) override {
			m_owner.onDownloadUpdated(browser, download_item, callback);
		}

	private:
		virtual CefResourceRequestHandler::ReturnValue
		OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefFrame> frame,
				CefRefPtr<CefRequest> request,
				CefRefPtr<CefCallback> callback) override {
			return m_ad_blocker->OnBeforeResourceLoad(
					browser, frame, request, callback);
		}

		virtual CefRefPtr<CefResourceRequestHandler>
		GetResourceRequestHandler(CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefFrame> frame,
				CefRefPtr<CefRequest> request,
				bool is_navigation,
				bool is_download,
				const CefString &request_initiator,
				bool &disable_default_handling) override {
			return m_ad_blocker;
		}

	private:
		GDBrowserView &m_owner;
		CefRefPtr<AdBlocker> m_ad_blocker;
		RoutingAudio m_audio;
	};

public:
	GDBrowserView();
	virtual ~GDBrowserView();

	inline int id() const {
		return m_id;
	}

	String getError();
	bool isValid() const;
	void setZoomLevel(double delta);
	void allowDownloads(bool allow);
	void downloadFile(String url);
	void setDownloadFolder(String folder);
	void loadURL(String url);
	void loadDataURI(String html, String mime_type);
	bool loaded() const;
	void stopLoading();
	bool reload() const;
	void copy() const;
	void paste() const;
	void cut() const;
	void delete_() const;
	void undo() const;
	void redo() const;
	void requestHtmlContent();
	void executeJavaScript(String javascript);
	void close();
	String getURL() const;
	String getTitle() const;

	inline Ref<ImageTexture> getTexture() {
		return m_texture;
	}

	inline void setTexture(Ref<ImageTexture> t) {
		m_texture = t;
	}

	bool canNavigateBackward() const;
	void navigateBackward();
	bool canNavigateForward() const;
	void navigateForward();

	inline void resize(const Vector2 &dim) {
		resize_(int(dim.x), int(dim.y));
	}

	bool viewport(float x, float y, float w, float h);
	void leftClick();
	void rightClick();
	void middleClick();
	void leftMouseUp();
	void rightMouseUp();
	void middleMouseUp();
	void leftMouseDown();
	void rightMouseDown();
	void middleMouseDown();
	void mouseMove(int x, int y);
	void mouseWheelVertical(const int wDelta);
	void mouseWheelHorizontal(const int wDelta);
	void keyPress(int key, bool pressed, bool shift, bool alt, bool ctrl);
	bool mute(bool state);
	bool muted();

	void
	setAudioStreamer(Ref<AudioStreamGeneratorPlayback> streamer) {
		if (m_impl != nullptr) {
			m_impl->m_audio.streamer = streamer;
		}
	}

	Ref<AudioStreamGeneratorPlayback> getAudioStreamer() {
		if (m_impl == nullptr) {
			return nullptr;
		}
		return m_impl->m_audio.streamer;
	}

	Color getPixelColor(int x, int y) const;
	bool registerGodotMethod(Object *object, String method_name);
	bool jsEmit(String event_name, const Variant &p_data);
	bool addAdBlockPattern(String pattern);
	void enableAdBlock(bool enable);
	bool isAdBlockEnabled() const;
	void log_info(String message);
	void log_warning(String message);
	void log_error(String message);
	void log_fatal(String message);

private:
	void resize_(int width, int height);
	int init(const String &url,
			CefBrowserSettings const &cef_settings,
			CefWindowInfo const &window_info);
	void getViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect);
	void onPaint(CefRefPtr<CefBrowser> browser,
			CefRenderHandler::PaintElementType type,
			const CefRenderHandler::RectList &dirtyRects,
			const void *buffer,
			int width,
			int height);
	void onLoadEnd(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			int httpStatusCode);
	void onLoadError(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const int errCode,
			const CefString &errorText);
	void onAudioStreamStarted(CefRefPtr<CefBrowser> browser,
			const CefAudioParameters &params,
			int channels);
	void onAudioStreamPacket(CefRefPtr<CefBrowser> browser,
			const float **p_data,
			int frames,
			int64_t pts);
	bool onBeforePopup(CefRefPtr<CefBrowser> browser,
			const CefString &target_url);
	bool canDownload(CefRefPtr<CefBrowser> browser,
			const CefString &url,
			const CefString &request_method);
	bool onBeforeDownload(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDownloadItem> download_item,
			const CefString &suggested_name,
			CefRefPtr<CefBeforeDownloadCallback> callback);
	void onDownloadUpdated(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDownloadItem> download_item,
			CefRefPtr<CefDownloadItemCallback> callback);
	bool onProcessMessageReceived(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message);
	Variant JsonToGodot(const Dictionary &json);
	Variant JsonToGodot(const Array &json_array);
	Variant JsonToGodot(const Variant &json_value);

private:
	friend GDBrowserView::Impl;

	CefRefPtr<Impl> m_impl = nullptr;
	CefRefPtr<CefBrowser> m_browser = nullptr;

	Ref<ImageTexture> m_texture;
	Ref<Image> m_image;
	PackedByteArray m_p_data;

	int m_mouse_x = 0;
	int m_mouse_y = 0;
	uint32_t m_mouse_event_modifiers = 0;
	int m_left_click_count = 1;
	std::chrono::system_clock::time_point m_last_left_down;
	float m_width = 128.0f;
	float m_height = 128.0f;
	std::array<float, 4> m_viewport;
	int m_id = -1;
	mutable std::stringstream m_error; // Kept for getError() method
	bool m_allow_downloads = true;
	fs::path m_download_folder;
	std::unordered_map<std::string, Callable> m_js_bindings;
};

#endif // GDCEF_BROWSER_HPP