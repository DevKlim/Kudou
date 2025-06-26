#include "audio_effect_panner.h"

void AudioEffectPannerInstance::process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count) {
	float lvol = CLAMP(1.0 - base->pan, 0, 1);
	float rvol = CLAMP(1.0 + base->pan, 0, 1);

	for (int i = 0; i < p_frame_count; i++) {
		p_dst_frames[i].left = p_src_frames[i].left * lvol + p_src_frames[i].right * (1.0 - rvol);
		p_dst_frames[i].right = p_src_frames[i].right * rvol + p_src_frames[i].left * (1.0 - lvol);
	}
}

Ref<AudioEffectInstance> AudioEffectPanner::instantiate() {
	Ref<AudioEffectPannerInstance> ins;
	ins.instantiate();
	ins->base = Ref<AudioEffectPanner>(this);
	return ins;
}

void AudioEffectPanner::set_pan(float p_cpanume) {
	pan = p_cpanume;
}

float AudioEffectPanner::get_pan() const {
	return pan;
}

void AudioEffectPanner::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_pan", "cpanume"), &AudioEffectPanner::set_pan);
	ClassDB::bind_method(D_METHOD("get_pan"), &AudioEffectPanner::get_pan);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pan", PROPERTY_HINT_RANGE, "-1,1,0.01"), "set_pan", "get_pan");
}

AudioEffectPanner::AudioEffectPanner() {
	pan = 0;
}
