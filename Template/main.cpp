#include <algorithm>
#include <ranges>
#include <span>
#include <string_view>
#include <print>

int main(int argc, char** argv) {
    auto args = std::span{argv, argv + argc} | std::views::transform([](char* str) { return std::string_view{str}; });
    std::ranges::for_each(args, [](std::string_view str){ std::print("{}", str); });
}
