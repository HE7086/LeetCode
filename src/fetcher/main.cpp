#include "question.hpp"

#include <charconv>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <print>
#include <string_view>

namespace fs = std::filesystem;

static constexpr auto const CODE_TEMPLATE = R"(/******************************
__DEBUG_FUNCTION<{}>__
Question {}: {}
Difficulty: {}

{}
URL: {}
******************************/

{}#include <gtest/gtest.h>

using namespace std;

//==============================================================================

{}

//==============================================================================

TEST(Test, {}) {{
  auto s = Solution{{}};
  EXPECT_EQ(0, 1);
}})";

static int64_t get_question_id(int argc, char* argv[]) {
  if (argc <= 1) {
    std::println(stderr, "Usage: {} <id>", argv[0]);
    return 0;
  }

  int64_t result = 0;

  auto const arg = std::string_view{argv[1]};
  auto [ptr, _]  = std::from_chars(arg.begin(), arg.end(), result);
  if (ptr != arg.end()) {
    std::println("Failed to parse Question ID: {}", arg);
    exit(1);
  }
  return result;
}

int main(int argc, char* argv[]) {
  auto const id   = get_question_id(argc, argv);
  auto const q    = question::get_by_id(id);
  auto const path = fs::current_path() / "src" / "solution" / (q.get_solution_id() + ".cpp");

  if (fs::exists(path) && !fs::is_empty(path)) {
    std::println(stderr, "{} already initialized! Skipping...", q.get_solution_id());
    return 0;
  }

  // write to solution folder
  auto file = std::ofstream(path);
  std::print(
      file,
      CODE_TEMPLATE,
      q.get_function(),
      id,
      q.get_title(),
      q.get_difficulty(),
      q.get_description(),
      q.get_url(),
      q.get_includes(),
      q.get_snippet(),
      q.get_solution_id()
  );

  std::println(stderr, "Successfully initialized {} at {}", id, path.filename().string());
  return 0;
}
