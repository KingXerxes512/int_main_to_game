#include "ResourceLoader.h"
#include "Error.h"

#include <fstream>
#include <ranges>

namespace
{

std::string load(const std::filesystem::path& name, auto openMode)
{
    std::ifstream file{name, openMode};
    game::ensure(!!file, "Could not open file");

    std::stringstream ss{};
    ss << file.rdbuf();

    return ss.str();
}

}

namespace game
{

ResourceLoader::ResourceLoader(const std::filesystem::path& root)
    : m_Root(root)
{
}

std::string ResourceLoader::Load_String(std::string_view name)
{
    return load(m_Root / name, std::ios::in);
}

std::vector<std::byte> ResourceLoader::Load_Binary(std::string_view name)
{
    const auto data = load(m_Root / name, std::ios::in | std::ios::binary);
    return data |                                                                    //
           std::views::transform([](auto b) { return static_cast<std::byte>(b); }) | //
           std::ranges::to<std::vector>();
}

}
