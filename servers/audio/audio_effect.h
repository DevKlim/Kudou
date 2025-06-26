#pragma once

#include "core/io/resource.h"
#include "core/math/audio_frame.h"
#include "core/object/gdvirtual.gen.inc"
#include "core/variant/native_ptr.h"

class AudioEffectInstance : public RefCounted {
	GDCLASS(AudioEffectInstance, RefCounted);

protected:
	GDVIRTUAL3_REQUIRED(_process, GDExtensionConstPtr<AudioFrame>, GDExtensionPtr<AudioFrame>, int)
	GDVIRTUAL0RC(bool, _process_silence)
	static void _bind_methods();

public:
	virtual void process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count);
	virtual bool process_silence() const;
};

class AudioEffect : public Resource {
	GDCLASS(AudioEffect, Resource);

protected:
	GDVIRTUAL0R_REQUIRED(Ref<AudioEffectInstance>, _instantiate)
	static void _bind_methods();

public:
	virtual Ref<AudioEffectInstance> instantiate();
	AudioEffect();
};
