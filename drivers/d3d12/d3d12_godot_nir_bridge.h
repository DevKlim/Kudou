#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

// This one leaves room for potentially extremely copious bindings in a set.
static const uint32_t GODOT_NIR_DESCRIPTOR_SET_MULTIPLIER = 100000000;
// This one leaves room for potentially big sized arrays.
static const uint32_t GODOT_NIR_BINDING_MULTIPLIER = 100000;

static const uint64_t GODOT_NIR_SC_SENTINEL_MAGIC = 0x45678900; // This must be as big as to be VBR-ed as a 32 bits number.
static const uint64_t GODOT_NIR_SC_SENTINEL_MAGIC_MASK = 0xffffffffffffff00;
static const uint64_t GODOT_NIR_SC_SENTINEL_ID_MASK = 0x00000000000000ff;

typedef struct GodotNirCallbacks {
	void *data;
	void (*report_resource)(uint32_t p_register, uint32_t p_space, uint32_t p_dxil_type, void *p_data);
	void (*report_sc_bit_offset_fn)(uint32_t p_sc_id, uint64_t p_bit_offset, void *p_data);
	void (*report_bitcode_bit_offset_fn)(uint64_t p_bit_offset, void *p_data);
} GodotNirCallbacks;

#ifdef __cplusplus
}
#endif
