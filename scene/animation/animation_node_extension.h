#pragma once

#include "scene/animation/animation_tree.h"

class AnimationNodeExtension : public AnimationNode {
	GDCLASS(AnimationNodeExtension, AnimationNode);

public:
	virtual NodeTimeInfo _process(const AnimationMixer::PlaybackInfo p_playback_info, bool p_test_only = false) override;

	static bool is_looping(const PackedFloat32Array &p_node_info);
	static double get_remaining_time(const PackedFloat32Array &p_node_info, bool p_break_loop = false);

protected:
	static void _bind_methods();

	GDVIRTUAL2R_REQUIRED(PackedFloat32Array, _process_animation_node, PackedFloat64Array, bool);

private:
	static AnimationNode::NodeTimeInfo _array_to_node_time_info(const PackedFloat32Array &p_array);
	static PackedFloat64Array _playback_info_to_array(const AnimationMixer::PlaybackInfo &p_playback_info);
};
