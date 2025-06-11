#include <cstdint>

#include <algorithm>
#include <charconv>
#include <filesystem>
#include <format>
#include <fstream>
#include <print>
#include <regex>
#include <span>

#include <cpr/cpr.h>
#include <cpr/response.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

static json build_graphql_payload(const std::string &titleSlug) {
  std::string query = R"(
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
    }
  )";
  json payload = {
      {"query",         query                     },
      {"variables",     {{"titleSlug", titleSlug}}},
      {"operationName", "questionData"            }
  };
  return payload;
}

static std::string code_template(uint64_t id, std::string title, std::string content, std::string url,
                                 std::string include, std::string code, std::string solution_id) {
  return std::format(R"(/******************************
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
  EXPECT_EQ(1, 1);
}})",
                     id,
                     title,
                     content,
                     url,
                     include,
                     code,
                     solution_id);
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
  auto entityMap = std::map<std::string, std::string>{
      {"&nbsp;",  ""  },
      {"&lt;",    "<" },
      {"&gt;",    ">" },
      {"&amp;",   "&" },
      {"&quot;",  "\""},
      {"&apos;",  "'" },
      {"&minus;", "-" },
      {"\n\n",    "\n"},
  };
  for (auto const &[entity, replacement] : entityMap) {
    size_t start_pos = 0;
    while ((start_pos = content.find(entity, start_pos)) != std::string::npos) {
      content.replace(start_pos, entity.length(), replacement);
      start_pos += replacement.length();
    }
  }
  return content;
}

static std::string add_return(std::string type, std::string code) {
  auto re = std::regex("\\{\n\\s*\\}");

  auto map = std::map<std::string, std::string>{
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

static std::string get_includes(std::vector<std::string> params) {
  auto result = std::string{};
  auto vector = false;
  auto list = false;

  for (const auto &param : params) {
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

int main(int argc, char *argv[]) {
  // parse args
  const auto args =
      std::span{argv, argv + argc} | std::views::transform([](const char *str) { return std::string_view(str); });
  if (args.size() <= 1) {
    std::println(stderr, "Usage: {} <id>", args[0]);
    return 0;
  }

  uint64_t question_id = 0;
  std::from_chars(args[1].begin(), args[1].end(), question_id);

  // check cache
  auto cache_path = fs::current_path().append(".cache");
  if (!fs::exists(cache_path)) {
    fs::create_directory(cache_path);
  }
  cache_path.append("problems.json");

  if (!fs::exists(cache_path)) {
    auto cache = std::ofstream(cache_path);
    auto r = cpr::Get(cpr::Url{"https://leetcode.com/api/problems/algorithms/"});
    std::print(cache, "{}", r.text);
    std::println(stderr, "Downloaded {} bytes in {} with status code {}", r.downloaded_bytes, r.elapsed, r.status_code);
    cache.close();
  }

  // map question id to title slug
  auto cache = std::ifstream(cache_path);
  json problems = json::parse(cache)["stat_status_pairs"];
  cache.close();

  auto question = std::find_if(problems.begin(), problems.end(), [question_id](const auto &p) {
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
  cpr::Response r = cpr::Post(
      cpr::Url{
          "https://leetcode.com/graphql"
  },
      cpr::Body{build_graphql_payload(title_slug).dump()},
      cpr::Header{{"Content-Type", "application/json"}});

#ifdef TEST
  std::println("{}", r.text);
#endif

  auto question_data = json::parse(r.text)["data"]["question"];
  if (question_data.value("isPaidOnly", false)) {
    std::println(stderr, "Note: paid-only: {}: {}", question_id, title_slug);
  }

  // get code snippets
  std::string code;
  for (const auto &snippet : question_data["codeSnippets"]) {
    if (snippet["lang"] == "C++") {
      code = snippet["code"];
      break;
    }
  }
  auto metaData = json::parse(question_data["metaData"].get<std::string>());
  auto params = std::vector<std::string>{};
  for (auto param : metaData["params"]) {
    params.emplace_back(param["type"]);
  }
  auto return_type = metaData["return"]["type"].get<std::string>();
  params.push_back(return_type);
  code = add_return(return_type, code);
  auto include = get_includes(params);
  auto content = parse_content(std::move(question_data["content"]));
  auto url = std::format("https://leetcode.com/problems/{}", title_slug);
  auto code_template_filled =
      code_template(question_id, stat["question__title"], content, url, include, code, solution_id);
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
