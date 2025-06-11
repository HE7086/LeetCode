#include <cstdint>

#include <algorithm>
#include <charconv>
#include <filesystem>
#include <format>
#include <fstream>
#include <print>
#include <regex>
#include <span>

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <cpr/response.h>

using json = nlohmann::json;
namespace fs = std::filesystem;

static constexpr const auto LEETCODE_API = "https://leetcode.com/api/problems/algorithms";
static constexpr const auto LEETCODE_GRAPHQL = "https://leetcode.com/graphql";
static constexpr const auto LEETCODE_PROBLEMS = "https://leetcode.com/problems";
static constexpr const auto GRAPHQL_QUERY = R"(
query questionData($titleSlug: String!) {
  question(titleSlug: $titleSlug) {
    content
    exampleTestcaseList
    metaData
    isPaidOnly
    codeSnippets {
      lang
      code
    }
  }
})";
static constexpr const auto CODE_TEMPLATE = R"(/******************************
Question {}: {}

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

static json build_graphql_payload(std::string const& titleSlug) {
  json payload = {
    {"query",         GRAPHQL_QUERY             },
    {"variables",     {{"titleSlug", titleSlug}}},
    {"operationName", "questionData"            }
  };
  return payload;
}

// assume input is well-formed html
static std::string parse_content(std::string content) {
  // erase all html tags
  while (content.find("<") != std::string::npos) {
    size_t start = content.find("<");
    size_t end = content.find(">");
    if (end != std::string::npos && end > start) {
      content.erase(start, (end - start) + 1);
    } else {
      break;
    }
  }

  // unescape characters
  static auto map = std::map<std::string, std::string>{
    {"&nbsp;",  ""  },
    {"&lt;",    "<" },
    {"&gt;",    ">" },
    {"&amp;",   "&" },
    {"&quot;",  "\""},
    {"&apos;",  "'" },
    {"&minus;", "-" },
    {"\n\n",    "\n"},
  };
  for (auto const& [entity, replacement] : map) {
    size_t start_pos = 0;
    while ((start_pos = content.find(entity, start_pos)) != std::string::npos) {
      content.replace(start_pos, entity.length(), replacement);
      start_pos += replacement.length();
    }
  }
  return content;
}

static std::string add_return(std::string const& type, std::string const& code) {
  static auto re = std::regex("\\{\n\\s*\\}");
  static auto map = std::map<std::string, std::string>{
    {"ListNode",            "{\n        return nullptr;\n    }"},
    {"ListNode[]",          "{\n        return {};\n    }"     },
    {"TreeNode",            "{\n        return nullptr;\n    }"},
    {"boolean",             "{\n        return false;\n    }"  },
    {"character",           "{\n        return '0';\n    }"    },
    {"character[][]",       "{\n        return {};\n    }"     },
    {"double",              "{\n        return 0;\n    }"      },
    {"double[]",            "{\n        return {};\n    }"     },
    {"int[]",               "{\n        return nullptr;\n    }"},
    {"integer",             "{\n        return 0;\n    }"      },
    {"integer[]",           "{\n        return {};\n    }"     },
    {"integer[][]",         "{\n        return {};\n    }"     },
    {"integer[][]",         "{\n        return {};\n    }"     },
    {"string",              "{\n        return \"\";\n    }"   },
    {"string[]",            "{\n        return {};\n    }"     },
    {"list<String>",        "{\n        return {};\n    }"     },
    {"list<string>",        "{\n        return {};\n    }"     },
    {"list<TreeNode>",      "{\n        return {};\n    }"     },
    {"list<boolean>",       "{\n        return {};\n    }"     },
    {"list<double>",        "{\n        return {};\n    }"     },
    {"list<integer>",       "{\n        return {};\n    }"     },
    {"list<list<integer>>", "{\n        return {};\n    }"     },
    {"list<list<string>>",  "{\n        return {};\n    }"     },
  };

  if (map.contains(type)) {
    return std::regex_replace(code, re, map[type]);
  } else {
    std::println(stderr, "Unknown return type: {}", type);
  }

  return code;
}

static std::string get_includes(std::vector<std::string> const& params) {
  auto result = std::string{};
  auto vector = false;
  auto list = false;

  for (auto const& param : params) {
    if (param.contains("ListNode")) {
      list = true;
    }
    if (param.contains("[]")) {
      vector = true;
    }
    if (param.contains("list<")) {
      vector = true;
    }
  }
  if (vector) {
    result += "#include <vector>\n";
  }
  if (list) {
    result += "#include <utils/list.hpp>\n";
  }

  return result;
}

static int64_t parse_args(int argc, char* argv[]) {
  const auto args =
    std::span{argv, argv + argc} | std::views::transform([](const char* str) { return std::string_view(str); });
  if (args.size() <= 1) {
    std::println(stderr, "Usage: {} <id>", args[0]);
    return 0;
  }

  int64_t result = 0;
  auto [ptr, _] = std::from_chars(args[1].begin(), args[1].end(), result);
  if (ptr != args[1].end()) {
    std::println("Failed to parse Question ID: {}", args[1]);
    exit(1);
  }
  return result;
}

static json get_problems() {
  auto const cache_path = fs::current_path().append(".cache").append("problems.json");

  if (!fs::exists(cache_path.parent_path())) {
    fs::create_directory(cache_path.parent_path());
  }
  if (!fs::exists(cache_path)) {
    auto cache = std::ofstream(cache_path);
    auto r = cpr::Get(cpr::Url{LEETCODE_API});
    std::print(cache, "{}", r.text);
    std::println(stderr, "Downloaded {} bytes in {} with status code {}", r.downloaded_bytes, r.elapsed, r.status_code);
    cache.close();
  }

  // map question id to title slug
  auto cache = std::ifstream(cache_path);
  json problems = json::parse(cache)["stat_status_pairs"];
  cache.close();

  return problems;
}

int main(int argc, char* argv[]) {
  auto question_id = parse_args(argc, argv);
  auto problems = get_problems();

  auto question = std::find_if(problems.begin(), problems.end(), [question_id](auto const& p) {
    return p["stat"]["question_id"] == question_id;
  });
  if (question == problems.end()) {
    std::println(stderr, "Question {} not found.", question_id);
    return 1;
  }

  auto stat = question.operator*()["stat"];
  auto title_slug = stat["question__title_slug"].get<std::string>();
  auto title_slug_snake = title_slug;
  std::replace(title_slug_snake.begin(), title_slug_snake.end(), '-', '_');

  // check if already initialized
  auto solution_id = std::format("s{:04}_{}", question_id, title_slug_snake);
  auto solution_file = std::format("{}.cpp", solution_id);
  auto solution_path = fs::current_path().append("src").append("solution").append(solution_file);
  if (fs::exists(solution_path)) {
    std::println(stderr, "{} already initialized! Skipping...", solution_file);
#ifndef TEST
    return 0;
#endif
  }

  // get question data
  // clang-format off
  cpr::Response r = cpr::Post(
    cpr::Url{LEETCODE_GRAPHQL},
    cpr::Body{build_graphql_payload(title_slug).dump()},
    cpr::Header{{"Content-Type", "application/json"}}
  );
  // clang-format on

#ifdef TEST
  std::println("{}", r.text);
#endif

  auto question_data = json::parse(r.text)["data"]["question"];
  if (question_data.value("isPaidOnly", false)) {
    std::println(stderr, "Note: paid-only: {}: {}", question_id, title_slug);
  }

  // get code snippets
  auto code = std::string{};
  for (auto const& snippet : question_data["codeSnippets"]) {
    if (snippet["lang"] == "C++") {
      code = snippet["code"];
      break;
      // assume must exist
    }
  }
  auto metaData = json::parse(question_data["metaData"].get<std::string>());
  // clang-format off
  auto params = metaData["params"]
    | std::views::transform([](auto const& p) { return p["type"]; })
    | std::ranges::to<std::vector<std::string>>();
  // clang-format on
  auto return_type = metaData["return"]["type"].get<std::string>();
  params.push_back(return_type);

  code = add_return(return_type, code);
  auto include = get_includes(params);
  auto content = parse_content(std::move(question_data["content"]));
  auto url = std::format("{}/{}", LEETCODE_PROBLEMS, title_slug);
  auto question_title = stat["question__title"].get<std::string>();
  auto code_template_filled =
    std::format(CODE_TEMPLATE, question_id, question_title, content, url, include, code, solution_id);
#ifdef TEST
  std::println("{}", code_template_filled);
  return 0;
#endif

  // write to solution folder
  auto solution = std::ofstream(solution_path);
  std::print(solution, "{}", code_template_filled);
  solution.close();

  std::println(stderr, "Successfully initialized {} at {}", question_id, solution_file);
  return 0;
}
