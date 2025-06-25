#pragma once

#include <format>
#include <mutex>
#include <print>
#include <source_location>

#define USE(x) ((void*)x)

namespace game::log
{

inline std::recursive_mutex log_mutex;

enum class Level
{
    DEBUG,
    INFO,
    WARN,
    ERR
};

template <Level L, class... Args>
struct Print
{
    Print(const char* msg, Args&&... args, std::source_location loc = std::source_location::current())
    {
        std::lock_guard lk(log_mutex);
        constexpr const char* logLevel = L == Level::DEBUG   ? "[DEBUG]"
                                         : L == Level::INFO  ? "[INFO] "
                                         : L == Level::WARN  ? "[WARN] "
                                         : L == Level::ERR ? "[ERROR]"
                                                             : "[?]";

        std::println(
            "{} {}:{} {}", logLevel, loc.file_name(), loc.line(), std::vformat(msg, std::make_format_args(args...)));
    }
};

// This allows for us to have Args... and also defaulted args at the end
// It basically takes all the given args to Args... and then leaves the defaulted arg to be default
template <Level L = Level::DEBUG, class... Args>
Print(const char*, Args&&...) -> Print<L, Args...>;

template <class... Args>
using debug = Print<Level::DEBUG, Args...>;

template <class... Args>
using info = Print<Level::INFO, Args...>;

template <class... Args>
using warn = Print<Level::WARN, Args...>;

template <class... Args>
using error = Print<Level::ERR, Args...>;

}
