#pragma once

#include <cstddef>
#include <filesystem>
#include <string_view>
#include <vector>

namespace game
{

class ResourceLoader
{
  public:
    ResourceLoader(const std::filesystem::path& root);

    std::string Load_String(std::string_view name);
    std::vector<std::byte> Load_Binary(std::string_view name);
  private:
    std::filesystem::path m_Root;
};

}
