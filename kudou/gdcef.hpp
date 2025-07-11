#ifndef GDCEF_HPP
#define GDCEF_HPP

#include "core/config/project_settings.h"
#include "core/object/class_db.h"
#include "core/os/os.h"
#include "scene/gui/texture_rect.h"

#include "cef_precomp.h"

class GDBrowserView;

class GDCef : public Node {
	GDCLASS(GDCef, Node);

public:
	void _init();
	void _exit_tree();
	bool initialize(Dictionary config);
	bool isAlive();
	String getError();
	void log_info(String message);
	void log_warning(String message);
	void log_error(String message);
	void log_fatal(String message);
	void _process(double delta);

protected:
	static void _bind_methods();

private:
	class Impl : public CefApp,
				 public CefLifeSpanHandler,
				 public CefClient {
	public:
		Impl(GDCef &cef) :
				m_owner(cef) {}
		void closeAllBrowsers(bool force_close);

	private:
		IMPLEMENT_REFCOUNTING(Impl);
		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
			return this;
		}

	private:
		virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
		virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
		virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

	private:
		GDCef &m_owner;
	};

public:
	~GDCef();
	void shutdown();
	String version();
	int versionPart(int entry);
	inline CefSettings const &settingsCEF() const {
		return m_cef_settings;
	}
	inline CefWindowInfo const &windowInfo() const {
		return m_window_info;
	}
	GDBrowserView *createBrowser(const String &url,
			TextureRect *texture_rect,
			Dictionary config);

private:
	friend GDCef::Impl;

	struct BrowsersSettings {
		bool enable_media_stream = false;
		std::string remote_allow_origin;
		bool enable_ad_block = true;
		Array custom_patterns;
		bool user_gesture_required = true;
		std::string user_agent;
	};

	CefRefPtr<GDCef::Impl> m_impl = nullptr;
	CefWindowInfo m_window_info;
	CefSettings m_cef_settings;
	BrowsersSettings m_browsers_settings;
	mutable std::stringstream m_error;
};

#endif