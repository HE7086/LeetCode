#pragma once

#include <cstdint>
#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class question {
  int64_t id;
  json    problem_data;
  json    question_data;
  json    meta_data;

public:
  question(int64_t id, json problem_data, json question_data);
  static question get_by_id(int64_t id);

  std::string get_function() const;
  std::string get_title() const;
  std::string get_difficulty() const;
  std::string get_description() const;
  std::string get_url() const;
  std::string get_includes() const;
  std::string get_snippet() const;
  std::string get_solution_id() const;
};
