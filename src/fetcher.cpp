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
#include <string_view>

#include <cpr/cpr.h>
#include <cpr/response.h>
#include <nlohmann/json.hpp>

using json   = nlohmann::json;
namespace fs = std::filesystem;

static constexpr auto const LEETCODE_API_URL      = "https://leetcode.com/api/problems/algorithms";
static constexpr auto const LEETCODE_GRAPHQL_URL  = "https://leetcode.com/graphql";
static constexpr auto const LEETCODE_PROBLEMS_URL = "https://leetcode.com/problems";
static constexpr auto const GRAPHQL_QUERY         = R"(
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
static constexpr auto const CODE_TEMPLATE         = R"(/******************************
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

static json build_graphql_payload(std::string_view titleSlug) {
  return {
      {"query",         GRAPHQL_QUERY             },
      {"variables",     {{"titleSlug", titleSlug}}},
      {"operationName", "questionData"            }
  };
}

static std::string add_return(std::string_view type, std::string code_snippet) {
  static auto const re  = std::regex("\\{\n\\s*\\}");
  static auto const map = std::map<std::string_view, std::string_view>{
      {"ListNode",            "{\n        return nullptr;\n    }"},
      {"ListNode[]",          "{\n        return {};\n    }"     },
      {"TreeNode",            "{\n        return nullptr;\n    }"},
      {"boolean",             "{\n        return false;\n    }"  },
      {"character",           "{\n        return '0';\n    }"    },
      {"character[][]",       "{\n        return {};\n    }"     },
      {"double",              "{\n        return 0;\n    }"      },
      {"double[]",            "{\n        return {};\n    }"     },
      {"int[]",               "{\n        return {};\n    }"     },
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

  if (auto const it = map.find(type); it != map.end()) {
    return std::regex_replace(code_snippet, re, it->second.data());
  }

  std::println(stderr, "Unknown return type: {}", type);
  return code_snippet;
}

static std::string get_code_snippet(json const& question_data) {
  auto const meta_data   = json::parse(question_data["metaData"].get<std::string>());
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
static std::string parse_question_desc(std::string desc) {
  // remove html tags
  static auto const re1 = std::regex{"<sup>"};
  static auto const re2 = std::regex{"<[^>]*>"};

  desc = std::regex_replace(desc, re1, "^");
  desc = std::regex_replace(desc, re2, "");

  // unescape characters
  static auto const map = std::map<std::string_view, std::string_view>{
      {"&nbsp;",  ""  },
      {"&lt;",    "<" },
      {"&gt;",    ">" },
      {"&amp;",   "&" },
      {"&quot;",  "\""},
      {"&apos;",  "'" },
      {"&minus;", "-" },
      {"\n\n",    "\n"},
      {"&#39;",   "'" },
      {"\t",      "  "},
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
  auto const meta_data = json::parse(question_data["metaData"].get<std::string_view>());

  auto has_vector = false;
  auto has_list   = false;
  auto has_tree   = false;

  auto process_type = [&](std::string_view type) {
    if (type.find("ListNode") != std::string_view::npos) {
      has_list = true;
    }
    if (type.find("TreeNode") != std::string_view::npos) {
      has_tree = true;
    }
    if (type.find("[]") != std::string_view::npos) {
      has_vector = true;
    }
    if (type.find("list<") != std::string_view::npos) {
      has_vector = true;
    }
  };

  for (auto const& param : meta_data["params"]) {
    process_type(param["type"].get<std::string_view>());
  }
  process_type(meta_data["return"]["type"].get<std::string_view>());

  auto result = std::string{};
  if (has_vector) {
    result += "#include <vector>\n";
  }
  if (has_list) {
    result += "#include <utils/list.hpp>\n";
  }
  if (has_tree) {
    result += "#include <utils/tree.hpp>\n";
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

static int64_t get_question_id(int argc, char* argv[]) {
  auto const args =
      std::span{argv, argv + argc} | std::views::transform([](char const* str) { return std::string_view(str); });
  if (args.size() <= 1) {
    std::println(stderr, "Usage: {} <id>", args[0]);
    return 0;
  }

  int64_t result = 0;
  auto [ptr, _]  = std::from_chars(args[1].begin(), args[1].end(), result);
  if (ptr != args[1].end()) {
    std::println("Failed to parse Question ID: {}", args[1]);
    exit(1);
  }
  return result;
}

int main(int argc, char* argv[]) {
  auto const question_id   = get_question_id(argc, argv);
  auto const question_stat = get_question_stat(get_problems(), question_id);

  auto const title_slug       = question_stat["question__title_slug"].get<std::string>();
  auto       title_slug_snake = title_slug;
  std::replace(title_slug_snake.begin(), title_slug_snake.end(), '-', '_');

  // check if already initialized
  auto const solution_id   = std::format("s{:04}_{}", question_id, title_slug_snake);
  auto const solution_path = fs::current_path() / "src" / "solution" / (solution_id + ".cpp");
  if (fs::exists(solution_path)) {
    std::println(stderr, "{} already initialized! Skipping...", solution_path.filename().string());
    exit(0);
  }

  // clang-format off
  cpr::Response r = cpr::Post(
    cpr::Url{LEETCODE_GRAPHQL_URL},
    cpr::Body{build_graphql_payload(title_slug).dump()},
    cpr::Header{{"Content-Type", "application/json"}}
  );
  // clang-format on

  auto const question_data = json::parse(r.text)["data"]["question"];
  if (question_data.value("isPaidOnly", false)) {
    std::println(stderr, "Note: paid-only: {}: {}", question_id, title_slug);
  }

  auto const question_title = question_stat["question__title"].get<std::string>();
  auto const question_desc  = parse_question_desc(question_data["content"]);
  auto const problem_url    = std::format("{}/{}", LEETCODE_PROBLEMS_URL, title_slug);
  auto const includes       = generate_includes(question_data);
  auto const code_snippet   = get_code_snippet(question_data);

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
