#include "include/cef_app.h"
#include <windows.h> // Required for UNREFERENCED_PARAMETER

// Entry point function for all processes.
int APIENTRY WinMain(HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPTSTR lpCmdLine,
		int nCmdShow) {
	// Suppress unused parameter warnings for secondary process entry point.
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	// Structure for passing command-line arguments.
	CefMainArgs main_args(hInstance);

	// Execute the secondary process logic.
	// For secondary processes (like render, GPU, utility), CefExecuteProcess
	// handles the process lifecycle based on command-line arguments passed
	// by the browser process. No CefApp or CefSettings are typically needed here.
	return CefExecuteProcess(main_args, nullptr, nullptr);
}