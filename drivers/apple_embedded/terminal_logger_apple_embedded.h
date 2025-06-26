#pragma once

#ifdef APPLE_EMBEDDED_ENABLED

#include "core/io/logger.h"

class TerminalLoggerAppleEmbedded : public StdLogger {
public:
	virtual void log_error(const char *p_function, const char *p_file, int p_line, const char *p_code, const char *p_rationale, bool p_editor_notify = false, ErrorType p_type = ERR_ERROR, const Vector<Ref<ScriptBacktrace>> &p_script_backtraces = {}) override;
};

#endif // APPLE_EMBEDDED_ENABLED
