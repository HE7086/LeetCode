#include "problem.hpp"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <print>

#include <cpr/cpr.h>
#include <cpr/response.h>

static constexpr auto const LEETCODE_API_URL = "https://leetcode.com/api/problems/algorithms";

namespace fs = std::filesystem;
using json   = nlohmann::json;

problem::problem() {
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
  data       = json::parse(cache)["stat_status_pairs"];
}

json problem::operator[](int64_t id) const {
  auto const problem_data = std::find_if(data.begin(), data.end(), [id](auto const& p) {
    return p["stat"]["question_id"] == id;
  });
  if (problem_data == data.end()) {
    std::println(stderr, "Question {} not found.", id);
    exit(1);
  }
  return *problem_data;
}
