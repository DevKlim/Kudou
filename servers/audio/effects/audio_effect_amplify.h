#pragma once

#include "servers/audio/audio_effect.h"

class AudioEffectAmplify;

class AudioEffectAmplifyInstance : public AudioEffectInstance {
	GDCLASS(AudioEffectAmplifyInstance, AudioEffectInstance);
	friend class AudioEffectAmplify;
	Ref<AudioEffectAmplify> base;

	float mix_volume_db;

public:
	virtual void process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count) override;
};

class AudioEffectAmplify : public AudioEffect {
	GDCLASS(AudioEffectAmplify, AudioEffect);

	friend class AudioEffectAmplifyInstance;
	float volume_db;

protected:
	static void _bind_methods();

public:
	Ref<AudioEffectInstance> instantiate() override;
	void set_volume_db(float p_volume);
	float get_volume_db() const;

	void set_volume_linear(float p_volume);
	float get_volume_linear() const;

	AudioEffectAmplify();
};
