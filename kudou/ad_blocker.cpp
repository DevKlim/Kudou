// Godot headers
#include "core/error/error_macros.h"
#include "helper_log.hpp"

// Undefs for CEF conflicts are now in ad_blocker.hpp

// CEF headers
#include "ad_blocker.hpp"

// Standard library headers
#include <algorithm> // For std::any_of
#include <regex> // For std::regex
#include <string> // For std::string
#include <vector> // For std::vector

//------------------------------------------------------------------------------
AdBlocker::AdBlocker() {
	GDCEF_DEBUG("");

	const std::vector<std::string> default_patterns = {
		// Classic advertising networks
		R"(.*doubleclick\.net.*)",
		R"(.*googlesyndication\.com.*)",
		R"(.*google-analytics\.com.*)",
		R"(.*adnxs\.com.*)",
		R"(.*advertising\.com.*)",
		R"(.*quantserve\.com.*)",
		R"(.*scorecardresearch\.com.*)",
		R"(.*zedo\.com.*)",
		R"(.*adbrite\.com.*)",
		R"(.*adocean\.pl.*)",
		R"(.*adsonar\.com.*)",
		R"(.*adtech\..*)",
		R"(.*adtechus\.com.*)",
		R"(.*atwola\.com.*)",
		R"(.*bidvertiser\.com.*)",
		R"(.*casalemedia\.com.*)",
		R"(.*chitika\.net.*)",
		R"(.*clicksor\.com.*)",
		R"(.*eclick\.vn.*)",
		R"(.*fmpub\.net.*)",
		R"(.*openx\..*)",
		R"(.*rubiconproject\.com.*)",
		R"(.*taboola\.com.*)",
		R"(.*outbrain\.com.*)",
		R"(.*criteo\..*)",
		R"(.*amazon-adsystem\.com.*)",
		R"(.*adform\..*)",
		R"(.*admob\..*)",
		R"(.*moatads\.com.*)",

		// Trackers et analytics
		R"(.*analytics\..*)",
		R"(.*tracking\..*)",
		R"(.*track\..*)",
		R"(.*stats\..*)",
		R"(.*pixel\..*)",
		R"(.*log\..*)",
		R"(.*beacon\..*)",
		R"(.*telemetry\..*)",
		R"(.*metrics\..*)",
		R"(.*matomo\..*)",
		R"(.*piwik\..*)",

		// Generic patterns
		R"(.*/ads/.*)",
		R"(.*/adserv.*)",
		R"(.*/banner.*)",
		// R"(.*/pop.*)", // See https://github.com/Lecrapouille/gdcef/issues/80
		R"(.*/sponsor.*)",
		R"(.*/advertising.*)",
		R"(.*/advert.*)",
		R"(.*/clicktrack.*)",
		R"(.*/affiliate.*)",
		R"(.*/promo.*)",
		R"(.*/commercials.*)",
		R"(.*/banners.*)",
		R"(.*/analytics.*)",
		R"(.*/tracker.*)",
		R"(.*/pixels.*)",
		R"(.*/count.*)",
		// R"(.*/stat.*)", // See
		// https://github.com/Lecrapouille/gdcef/issues/80
		R"(.*/targeting.*)",
		R"(.*/adview.*)",
		R"(.*/adclick.*)",

		// Social networks tracking
		R"(.*facebook\.com/tr.*)",
		R"(.*facebook\.com/plugins.*)",
		R"(.*linkedin\.com/pixel.*)",
		R"(.*twitter\.com/i/jot.*)",
		R"(.*pinterest\.com/ping.*)",

		// Other specific trackers
		R"(.*hotjar\.com.*)",
		R"(.*mouseflow\.com.*)",
		R"(.*crazyegg\.com.*)",
		R"(.*clicktale\.net.*)",
		R"(.*optimizely\.com.*)",
		R"(.*mixpanel\.com.*)",
		R"(.*kissmetrics\.com.*)",
		R"(.*segment\.io.*)",
		R"(.*segment\.com.*)",
		R"(.*amplitude\.com.*)",
		R"(.*bugsnag\.com.*)",
		R"(.*sentry\.io.*)",
		R"(.*newrelic\.com.*)"
	};

	for (const auto &pattern : default_patterns) {
		addPattern(pattern);
	}
}

//------------------------------------------------------------------------------
void AdBlocker::enable(bool enable) {
	m_enabled = enable;
}

//------------------------------------------------------------------------------
bool AdBlocker::addPattern(const std::string &pattern) {
	GDCEF_DEBUG("Adding ad blocking pattern: " << pattern);
	try {
		m_patterns.push_back(std::regex(pattern, std::regex::icase));
		return true;
	} catch (const std::regex_error &e) {
		GDCEF_ERROR("Invalid ad blocking pattern: " << pattern << " (" << e.what() << ")");
		return false;
	}
}

//------------------------------------------------------------------------------
CefResourceRequestHandler::ReturnValue
AdBlocker::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		CefRefPtr<CefCallback> callback) {
	std::string url = request->GetURL().ToString();
	if (m_enabled) {
		if (std::any_of(m_patterns.begin(), m_patterns.end(), [&](const std::regex &pattern) {
				return std::regex_match(url, pattern);
			})) {
			GDCEF_DEBUG("Blocked ad URL: " << url);
			return RV_CANCEL; // Block the request
		}
	}

	// GDCEF_DEBUG("Allowed ad URL: " << url);
	return RV_CONTINUE; // Allow the request
}