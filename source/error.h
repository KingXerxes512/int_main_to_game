#pragma once

#include <format>
#include <string_view>

#include "AutoRelease.h"
#include "Exception.h"

namespace game
{

template <class... Args>
inline void ensure(bool predicate, std::string_view msg, Args&&... args)
{
    if (!predicate)
    {
        constexpr std::uint32_t stacktrace_skip = 2;
        throw Exception(std::vformat(msg, std::make_format_args(std::forward<Args>(args)...)), stacktrace_skip);
    }
}

template <class T, T Invalid, class... Args>
inline void ensure(AutoRelease<T, Invalid>& obj, std::string_view msg, Args&&... args)
{
    ensure(!!obj, msg, std::forward<Args>(args)...);
}

}
