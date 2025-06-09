#pragma once

#include <stddef.h>
#include <string>
#include <string_view>
#include <unordered_map>

// All of this exists so that you can have a std::unordered_map with Key=std::string and use find with a string_view
// param and have it not create a std::string from the view

namespace game
{

struct StringHash
{
    using is_transparent = void;
    std::size_t operator()(const char* str) const
    {
        return std::hash<std::string_view>{}(str);
    }

    std::size_t operator()(std::string_view str) const
    {
        return std::hash<std::string_view>{}(str);
    }

    std::size_t operator()(const std::string& str) const
    {
        return std::hash<std::string_view>{}(str);
    }
};

template <class T>
using StringMap = std::unordered_map<std::string, T, StringHash, std::equal_to<>>;

}
