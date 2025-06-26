#pragma once

#include "core/input/input.h"
#include "servers/display_server.h"

#define FontVariation __FontVariation

#import <AppKit/AppKit.h>

#undef FontVariation

class DisplayServerMacOSBase : public DisplayServer {
	GDSOFTCLASS(DisplayServerMacOSBase, DisplayServer)

	id tts = nullptr;

protected:
	_THREAD_SAFE_CLASS_

	void initialize_tts() const;

public:
	virtual void clipboard_set(const String &p_text) override;
	virtual String clipboard_get() const override;
	virtual Ref<Image> clipboard_get_image() const override;
	virtual bool clipboard_has() const override;
	virtual bool clipboard_has_image() const override;

	virtual bool tts_is_speaking() const override;
	virtual bool tts_is_paused() const override;
	virtual TypedArray<Dictionary> tts_get_voices() const override;

	virtual void tts_speak(const String &p_text, const String &p_voice, int p_volume = 50, float p_pitch = 1.f, float p_rate = 1.f, int p_utterance_id = 0, bool p_interrupt = false) override;
	virtual void tts_pause() override;
	virtual void tts_resume() override;
	virtual void tts_stop() override;

	DisplayServerMacOSBase();
};
