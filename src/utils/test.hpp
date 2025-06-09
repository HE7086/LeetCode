#include <concepts>
#include <cstdlib>
#include <format>
#include <print>
#include <source_location>
#include <string_view>

template <typename A, typename B, typename CharT = char>
  requires(std::equality_comparable_with<A, B>
      && std::formattable<A, CharT>
      && std::formattable<B, CharT>)
void ASSERT_EQ(
    const A& expected,
    const B& actual,
    const std::source_location loc = std::source_location::current()) {
  if (expected != actual) {
    std::string_view filename = loc.file_name();
    filename.remove_prefix(filename.find_last_of('/') + 1);
    std::println(stderr, "{}:{}: {} != {}",
        filename,
        loc.line(),
        expected,
        actual);
    exit(1);
  }
}
