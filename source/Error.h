#pragma once

#include "AutoRelease.h"
#include "Exception.h"

#include <format>
#include <memory>
#include <string_view>

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
    ensure(!!obj, msg, std::forward<Args&&>(args)...);
}

template <class T, class D, class... Args>
inline void ensure(std::unique_ptr<T, D>& obj, std::string_view msg, Args&&... args)
{
    ensure(!!obj, msg, std::forward<Args&&>(args)...);
}

}
