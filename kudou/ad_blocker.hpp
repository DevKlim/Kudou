#ifndef AD_BLOCKER_HPP
#define AD_BLOCKER_HPP

// This is a workaround for an enum name collision between Godot's Error enum
// (core/error/error_list.h) and CEF's net error list. We define the conflicting
// names as macros before including the CEF header. This causes the preprocessor
// to rename them during CEF header parsing, avoiding the conflict.
#define ERR_FILE_NOT_FOUND CEF_ERR_FILE_NOT_FOUND
#define ERR_OUT_OF_MEMORY CEF_ERR_OUT_OF_MEMORY

#include "include/cef_resource_request_handler.h"
#include <regex>
#include <string>
#include <vector>

// We undefine the macros immediately after to prevent them from interfering
// with any other code.
#undef ERR_FILE_NOT_FOUND
#undef ERR_OUT_OF_MEMORY

class AdBlocker : public CefResourceRequestHandler {
public:
	AdBlocker();

	void enable(bool enable);

	inline bool is_enabled() const {
		return m_enabled;
	}

	bool addPattern(const std::string &pattern);

	CefResourceRequestHandler::ReturnValue
	OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			CefRefPtr<CefCallback> callback) override;

	IMPLEMENT_REFCOUNTING(AdBlocker);

private:
	std::vector<std::regex> m_patterns;
	bool m_enabled = true;
};

#endif