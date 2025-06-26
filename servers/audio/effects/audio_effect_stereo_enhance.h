#pragma once

#include "servers/audio/audio_effect.h"

class AudioEffectStereoEnhance;

class AudioEffectStereoEnhanceInstance : public AudioEffectInstance {
	GDCLASS(AudioEffectStereoEnhanceInstance, AudioEffectInstance);
	friend class AudioEffectStereoEnhance;
	Ref<AudioEffectStereoEnhance> base;

	enum {
		MAX_DELAY_MS = 50
	};

	float *delay_ringbuff = nullptr;
	unsigned int ringbuff_pos = 0;
	unsigned int ringbuff_mask = 0;

public:
	virtual void process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count) override;

	~AudioEffectStereoEnhanceInstance();
};

class AudioEffectStereoEnhance : public AudioEffect {
	GDCLASS(AudioEffectStereoEnhance, AudioEffect);

	friend class AudioEffectStereoEnhanceInstance;
	float volume_db = 0.0f;

	float pan_pullout = 1.0f;
	float time_pullout = 0.0f;
	float surround = 0.0f;

protected:
	static void _bind_methods();

public:
	Ref<AudioEffectInstance> instantiate() override;

	void set_pan_pullout(float p_amount);
	float get_pan_pullout() const;

	void set_time_pullout(float p_amount);
	float get_time_pullout() const;

	void set_surround(float p_amount);
	float get_surround() const;

	AudioEffectStereoEnhance();
};
