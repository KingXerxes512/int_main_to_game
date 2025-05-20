#pragma once

#include <format>
#include <mutex>
#include <print>
#include <source_location>

namespace game::log
{

inline std::recursive_mutex log_mutex;

enum class Level
{
    DEBUG,
    INFO,
    WARN,
    ERROR
};

template <Level L, class... Args>
struct print
{
};

template <Level L, class... Args>
struct print<L, const char*, Args...>
{
    print(const char* msg, const Args&... args, std::source_location loc = std::source_location::current())
    {
        std::lock_guard lk(log_mutex);
        constexpr const char* c = L == Level::DEBUG   ? "[DEBUG]"
                                  : L == Level::INFO  ? "[INFO] "
                                  : L == Level::WARN  ? "[WARN] "
                                  : L == Level::ERROR ? "[ERROR]"
                                                      : "[?]";

        std::println("{} {}:{} {}", c, loc.file_name(), loc.line(), std::vformat(msg, std::make_format_args(args...)));
    }
};

// This allows for us to have Args... and also defaulted args at the end
// It basically takes all the given args to Args... and then leaves the defaulted arg to be default
template <Level L, class... Args>
print(Args...) -> print<L, Args...>;

template <class... Args>
using debug = print<Level::DEBUG, Args...>;

template <class... Args>
using info = print<Level::INFO, Args...>;

template <class... Args>
using warn = print<Level::WARN, Args...>;

template <class... Args>
using error = print<Level::ERROR, Args...>;

}
