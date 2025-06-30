#include "question.hpp"
#include "problem.hpp"

#include <algorithm>
#include <cstdint>
#include <format>
#include <print>
#include <regex>
#include <string_view>
#include <utility>

#include <cpr/cpr.h>
#include <cpr/response.h>

using json = nlohmann::json;

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

static json get_question_data(std::string_view title_slug) {
  auto const query = json{
      {"query",         GRAPHQL_QUERY              },
      {"variables",     {{"titleSlug", title_slug}}},
      {"operationName", "questionData"             }
  };

  // clang-format off
  cpr::Response r = cpr::Post(
    cpr::Url{LEETCODE_GRAPHQL_URL},
    cpr::Body{query.dump()},
    cpr::Header{{"Content-Type", "application/json"}}
  );
  // clang-format on

  return json::parse(r.text)["data"]["question"];
}

question question::get_by_id(int64_t id) {
  auto problem_data  = problem{}[id];
  auto title_slug    = problem_data["stat"]["question__title_slug"].get<std::string>();
  auto question_data = get_question_data(title_slug);

  return question{id, problem_data, question_data};
}

question::question(int64_t id, json problem_data, json question_data)
    : id(id), problem_data(std::move(problem_data)), question_data(std::move(question_data)) {
  meta_data = json::parse(this->question_data["metaData"].get<std::string>());
}

std::string question::get_function() const {
  return meta_data["name"];
}

std::string question::get_title() const {
  return problem_data["stat"]["question__title"];
}

std::string question::get_difficulty() const {
  static constexpr auto const to_str = [](int diff) {
    switch (diff) {
      case 1: return "Easy";
      case 2: return "Med.";
      case 3: return "Hard";
      default: return "Unknown";
    }
  };
  return to_str(problem_data["difficulty"]["level"]);
}

std::string question::get_description() const {
  // assume input is well-formed html
  auto desc = question_data["content"].get<std::string>();
  // remove html tags
  static auto const re1 = std::regex{"<sup>"};
  static auto const re2 = std::regex{"<[^>]*>"};

  desc = std::regex_replace(desc, re1, "^");
  desc = std::regex_replace(desc, re2, "");

  // unescape characters
  static auto const map = std::map<std::string_view, std::string_view>{
      {"&nbsp;",  " " },
      {"\u200b",  " " },
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

std::string question::get_url() const {
  return std::format("{}/{}", LEETCODE_PROBLEMS_URL, problem_data["stat"]["question__title_slug"].get<std::string>());
}

std::string question::get_includes() const {
  auto has_vector = false;
  auto has_string = false;
  auto has_list   = false;
  auto has_tree   = false;

  auto process_type = [&](std::string_view type) {
    if (type.contains("ListNode")) {
      has_list = true;
    }
    if (type.contains("TreeNode")) {
      has_tree = true;
    }
    if (type.contains("[]")) {
      has_vector = true;
    }
    if (type.contains("list<")) {
      has_vector = true;
    }
    if (type.contains("String")) {
      has_string = true;
    }
    if (type.contains("string")) {
      has_string = true;
    }
  };

  for (auto const& param : meta_data["params"]) {
    process_type(param["type"].get<std::string_view>());
  }
  process_type(meta_data["return"]["type"].get<std::string_view>());

  auto result = std::string{};
  if (has_string) {
    result += "#include <string>\n";
  }
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
      {"void",                "{\n        return;\n    }"        },
  };

  if (auto it = map.find(type); it != map.end()) {
    return std::regex_replace(code_snippet, re, it->second.data());
  }

  std::println(stderr, "Unknown return type: {}", type);
  return code_snippet;
}

std::string question::get_snippet() const {
  auto snippets    = question_data["codeSnippets"];
  auto return_type = meta_data["return"]["type"].get<std::string>();

  for (auto const& snippet : snippets) {
    if (snippet["lang"] == "C++") {
      return add_return(return_type, snippet["code"]);
    }
  }
  std::println(stderr, "Question does not have C++ variant.");
  exit(1);
}

std::string question::get_solution_id() const {
  auto title_slug = problem_data["stat"]["question__title_slug"].get<std::string>();
  std::replace(title_slug.begin(), title_slug.end(), '-', '_');
  return std::format("s{:04}_{}", id, title_slug);
}
