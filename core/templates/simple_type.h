#pragma once

#include <type_traits>

template <typename T>
using GetSimpleTypeT = typename std::remove_cv_t<std::remove_reference_t<T>>;
