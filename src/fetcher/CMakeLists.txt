add_executable(fetcher main.cpp question.cpp)
target_link_libraries(fetcher PRIVATE
  nlohmann_json::nlohmann_json
  cpr::cpr
)
target_compile_options(fetcher PRIVATE -Wall -Wextra -Wpedantic -Werror)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(fetcher PRIVATE -Wnrvo)
endif()
set_property(TARGET fetcher PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
set_property(TARGET fetcher PROPERTY POSITION_INDEPENDENT_CODE TRUE)
set_property(TARGET fetcher PROPERTY EXCLUDE_FROM_ALL TRUE)
