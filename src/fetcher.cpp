#include <cstdint>

#include <algorithm>
#include <charconv>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <print>
#include <regex>
#include <span>

#include <nlohmann/json.hpp>
#include <utility>
#include <cpr/cpr.h>
#include <cpr/response.h>

using json = nlohmann::json;
namespace fs = std::filesystem;

static constexpr auto const LEETCODE_API_URL = "https://leetcode.com/api/problems/algorithms";
static constexpr auto const LEETCODE_GRAPHQL_URL = "https://leetcode.com/graphql";
static constexpr auto const LEETCODE_PROBLEMS_URL = "https://leetcode.com/problems";
static constexpr auto const GRAPHQL_QUERY = R"(
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
static constexpr auto const CODE_TEMPLATE = R"(/******************************
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
  return {
    {"query",         GRAPHQL_QUERY             },
    {"variables",     {{"titleSlug", titleSlug}}},
    {"operationName", "questionData"            }
  };
}

static std::string add_return(std::string const& type, std::string const& code_snippet) {
  static auto const re = std::regex("\\{\n\\s*\\}");
  static auto const map = std::map<std::string, std::string>{
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
    return std::regex_replace(code_snippet, re, map.at(type));
  }

  std::println(stderr, "Unknown return type: {}", type);
  return code_snippet;
}

static std::string get_code_snippet(json const& question_data) {
  auto const meta_data = json::parse(question_data["metaData"].get<std::string>());
  auto const return_type = meta_data["return"]["type"].get<std::string>();
  for (auto const& snippet : question_data["codeSnippets"]) {
    if (snippet["lang"] == "C++") {
      return add_return(return_type, snippet["code"]);
    }
  }
  std::println(stderr, "Question does not have C++ variant.");
  exit(1);
}

// assume input is well-formed html
static std::string parse_question_desc(std::string&& desc) {
  // remove html tags
  static auto const re = std::regex{"<[^>]*>"};
  desc = std::regex_replace(desc, re, "");

  // unescape characters
  static auto const map = std::map<std::string, std::string>{
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
    while ((start_pos = desc.find(entity, start_pos)) != std::string::npos) {
      desc.replace(start_pos, entity.length(), replacement);
      start_pos += replacement.length();
    }
  }
  return desc;
}

static std::string generate_includes(json const& question_data) {
  auto const meta_data = json::parse(question_data["metaData"].get<std::string>());
  // clang-format off
  auto types = meta_data["params"]
    | std::views::transform([](auto const& p) { return p["type"]; })
    | std::ranges::to<std::vector<std::string>>();
  // clang-format on
  types.emplace_back(meta_data["return"]["type"]);

  auto result = std::string{};
  auto vector = false;
  auto list = false;

  for (auto const& type : types) {
    if (type.contains("ListNode")) {
      list = true;
    }
    if (type.contains("[]")) {
      vector = true;
    }
    if (type.contains("list<")) {
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

static json get_question_stat(json const& problems, int64_t question_id) {
  auto const question = std::find_if(problems.begin(), problems.end(), [question_id](auto const& p) {
    return p["stat"]["question_id"] == question_id;
  });
  if (question == problems.end()) {
    std::println(stderr, "Question {} not found.", question_id);
    exit(1);
  }
  return question.operator*()["stat"];
}

static json get_problems() {
  auto const cache_path = fs::current_path() / ".cache" / "problems.json";

  if (!fs::exists(cache_path.parent_path())) {
    fs::create_directory(cache_path.parent_path());
  }

  if (!fs::exists(cache_path)) {
    auto const r = cpr::Get(cpr::Url{LEETCODE_API_URL});
    std::println(stderr, "Downloaded {} bytes in {} with status code {}", r.downloaded_bytes, r.elapsed, r.status_code);

    auto cache = std::ofstream(cache_path);
    std::print(cache, "{}", r.text);
  }

  auto cache = std::ifstream(cache_path);
  return json::parse(cache)["stat_status_pairs"];
}

static int64_t parse_question_id(int argc, char* argv[]) {
  auto const args =
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

int main(int argc, char* argv[]) {
  auto const question_id = parse_question_id(argc, argv);
  auto const question_stat = get_question_stat(get_problems(), question_id);

  auto const title_slug = question_stat["question__title_slug"].get<std::string>();
  auto title_slug_snake = title_slug;
  std::replace(title_slug_snake.begin(), title_slug_snake.end(), '-', '_');

  // check if already initialized
  auto const solution_id = std::format("s{:04}_{}", question_id, title_slug_snake);
  auto const solution_path = fs::current_path() / "src" / "solution" / (solution_id + ".cpp");
  if (fs::exists(solution_path)) {
    std::println(stderr, "{} already initialized! Skipping...", solution_path.filename().string());
    exit(0);
  }

  cpr::Response r = cpr::Post(
    // clang-format off
    cpr::Url{LEETCODE_GRAPHQL_URL},
    cpr::Body{build_graphql_payload(title_slug).dump()},
    cpr::Header{{"Content-Type", "application/json"}}
    // clang-format on
  );

  auto const question_data = json::parse(r.text)["data"]["question"];
  if (question_data.value("isPaidOnly", false)) {
    std::println(stderr, "Note: paid-only: {}: {}", question_id, title_slug);
  }

  auto const question_title = question_stat["question__title"].get<std::string>();
  auto const question_desc = parse_question_desc(std::move(question_data["content"]));
  auto const problem_url = std::format("{}/{}", LEETCODE_PROBLEMS_URL, title_slug);
  auto const includes = generate_includes(question_data);
  auto const code_snippet = get_code_snippet(question_data);

  // write to solution folder
  auto solution = std::ofstream(solution_path);
  std::print(
    solution,
    CODE_TEMPLATE,
    question_id,
    question_title,
    question_desc,
    problem_url,
    includes,
    code_snippet,
    solution_id
  );

  std::println(stderr, "Successfully initialized {} at {}", question_id, solution_path.filename().string());
  return 0;
}
