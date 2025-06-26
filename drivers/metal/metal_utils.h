#pragma once

#import <os/log.h>

#import <functional>

#pragma mark - Boolean flags

namespace flags {

/*! Sets the flags within the value parameter specified by the mask parameter. */
template <typename Tv, typename Tm>
void set(Tv &p_value, Tm p_mask) {
	using T = std::underlying_type_t<Tv>;
	p_value = static_cast<Tv>(static_cast<T>(p_value) | static_cast<T>(p_mask));
}

/*! Clears the flags within the value parameter specified by the mask parameter. */
template <typename Tv, typename Tm>
void clear(Tv &p_value, Tm p_mask) {
	using T = std::underlying_type_t<Tv>;
	p_value = static_cast<Tv>(static_cast<T>(p_value) & ~static_cast<T>(p_mask));
}

/*! Returns whether the specified value has any of the bits specified in mask set to 1. */
template <typename Tv, typename Tm>
static constexpr bool any(Tv p_value, const Tm p_mask) {
	return ((p_value & p_mask) != 0);
}

/*! Returns whether the specified value has all of the bits specified in mask set to 1. */
template <typename Tv, typename Tm>
static constexpr bool all(Tv p_value, const Tm p_mask) {
	return ((p_value & p_mask) == p_mask);
}

} //namespace flags

#pragma mark - Alignment and Offsets

static constexpr bool is_power_of_two(uint64_t p_value) {
	return p_value && ((p_value & (p_value - 1)) == 0);
}

static constexpr uint64_t round_up_to_alignment(uint64_t p_value, uint64_t p_alignment) {
	DEV_ASSERT(is_power_of_two(p_alignment));

	if (p_alignment == 0) {
		return p_value;
	}

	uint64_t mask = p_alignment - 1;
	uint64_t aligned_value = (p_value + mask) & ~mask;

	return aligned_value;
}

class Defer {
public:
	Defer(std::function<void()> func) :
			func_(func) {}
	~Defer() { func_(); }

private:
	std::function<void()> func_;
};

#define CONCAT_INTERNAL(x, y) x##y
#define CONCAT(x, y) CONCAT_INTERNAL(x, y)
#define DEFER const Defer &CONCAT(defer__, __LINE__) = Defer

extern os_log_t LOG_DRIVER;
// Used for dynamic tracing.
extern os_log_t LOG_INTERVALS;
