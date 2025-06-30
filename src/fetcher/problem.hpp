#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>

class problem {
  nlohmann::json data;

public:
  problem();

  [[nodiscard]] nlohmann::json operator[](int64_t id) const;
};
