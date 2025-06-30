#pragma once

#include <cstdint>
#include <string>

#include <nlohmann/json.hpp>

class question {
  int64_t        id;
  nlohmann::json problem_data;
  nlohmann::json question_data;
  nlohmann::json meta_data;

public:
  question(int64_t id, nlohmann::json problem_data, nlohmann::json question_data);
  [[nodiscard]] static question get_by_id(int64_t id);

  [[nodiscard]] std::string get_function() const;
  [[nodiscard]] std::string get_title() const;
  [[nodiscard]] std::string get_difficulty() const;
  [[nodiscard]] std::string get_description() const;
  [[nodiscard]] std::string get_url() const;
  [[nodiscard]] std::string get_includes() const;
  [[nodiscard]] std::string get_snippet() const;
  [[nodiscard]] std::string get_solution_id() const;
};
