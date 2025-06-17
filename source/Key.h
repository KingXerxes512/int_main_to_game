#pragma once

#include <format>
#include <string>

#include <windows.h>

namespace game
{

enum class Key
{
    ENTER = 0x0D,
    ESC = 0x1B,
    SPACE = 0x20,
    A = 0x41,
    B = 0x42,
    C = 0x43,
    D = 0x44,
    E = 0x45,
    F = 0x46,
    G = 0x47,
    H = 0x48,
    I = 0x49,
    J = 0x4A,
    K = 0x4B,
    L = 0x4C,
    M = 0x4D,
    N = 0x4E,
    O = 0x4F,
    P = 0x50,
    Q = 0x51,
    R = 0x52,
    S = 0x53,
    T = 0x54,
    U = 0x55,
    V = 0x56,
    W = 0x57,
    X = 0x58,
    Y = 0x59,
    Z = 0x5A,
    F1 = VK_F1,
};

enum class KeyState
{
    UP,
    DOWN
};

}

template <>
struct std::formatter<game::Key>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return std::begin(ctx);
    }

    auto format(const game::Key& obj, std::format_context& ctx) const
    {
        using namespace std::literals;
        using enum game::Key;
        auto s = "?"sv;
        switch (obj)
        {
            case ENTER: s = "ENTER"; break;
            case ESC: s = "ESC"; break;
            case SPACE: s = "SPACE"; break;
            case A: s = "A"; break;
            case B: s = "B"; break;
            case C: s = "C"; break;
            case D: s = "D"; break;
            case E: s = "E"; break;
            case F: s = "F"; break;
            case G: s = "G"; break;
            case H: s = "H"; break;
            case I: s = "I"; break;
            case J: s = "J"; break;
            case K: s = "K"; break;
            case L: s = "L"; break;
            case M: s = "M"; break;
            case N: s = "N"; break;
            case O: s = "O"; break;
            case P: s = "P"; break;
            case Q: s = "Q"; break;
            case R: s = "R"; break;
            case S: s = "S"; break;
            case T: s = "T"; break;
            case U: s = "U"; break;
            case V: s = "V"; break;
            case W: s = "W"; break;
            case X: s = "X"; break;
            case Y: s = "Y"; break;
            case Z: s = "Z"; break;
        }

        return std::format_to(ctx.out(), "{}", s);
    }
};

template <>
struct std::formatter<game::KeyState>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return std::begin(ctx);
    }

    auto format(const game::KeyState& obj, std::format_context& ctx) const
    {
        using namespace std::literals;
        using enum game::KeyState;
        auto s = "?"sv;
        switch (obj)
        {
            case UP: return std::format_to(ctx.out(), "UP");
            case DOWN: return std::format_to(ctx.out(), "DOWN"); ;
            default: return std::format_to(ctx.out(), "?");
        }
    }
};
