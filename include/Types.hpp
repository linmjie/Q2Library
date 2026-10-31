#pragma once

#include <span>
#include <string>
#include <string_view>
#include <vector>

using u64 = unsigned long long;

// using string_view is technically more efficient
// but it's easier to regex with const std::string &
using Str = std::string_view;

using String = std::string;

template <typename T> using Span = std::span<const T>;

template <typename T> using Vec = std::vector<T>;
