include(FetchContent)

#==================== solution ====================

find_package(GTest)
if(NOT GTest_FOUND)
  FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
  )
  FetchContent_MakeAvailable(googletest)
endif()

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/solution/*.cpp")
foreach(source_file ${SOURCE_FILES})
  get_filename_component(exe ${source_file} NAME_WE)
  string(REGEX MATCH "s([0-9]+)_.*" _ ${exe})
  add_executable(${exe} ${source_file})
  target_link_libraries(${exe} PRIVATE GTest::GTest GTest::Main)
  target_include_directories(${exe} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
  gtest_discover_tests(${exe})

  if(NOT DEFINED ENV{GITHUB_ACTIONS})
    # disable warnings on ci
    target_compile_options(${exe} PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter)
    target_compile_options(${exe} PRIVATE -fsanitize=address -fsanitize=undefined)
    target_link_options(${exe} PRIVATE -fsanitize=address -fsanitize=undefined)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      target_compile_options(${exe} PRIVATE -Wnrvo)
    endif()
  endif()
endforeach()

#==================== fetcher ====================

if(NOT DEFINED ENV{GITHUB_ACTIONS})
# do not build fetcher in ci

find_package(nlohmann_json)
if(NOT nlohmann_json_FOUND)
  FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
  )
  FetchContent_MakeAvailable(json)
endif()

find_package(cpr)
if(NOT cpr_FOUND)
  FetchContent_Declare(cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.11.2
  )
  FetchContent_MakeAvailable(cpr)
endif()

add_executable(fetcher fetcher.cpp)
target_link_libraries(fetcher PRIVATE
  nlohmann_json::nlohmann_json
  cpr::cpr
)
target_compile_options(fetcher PRIVATE -Wall -Wextra -Wpedantic -Werror)
set_property(TARGET fetcher PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
set_property(TARGET fetcher PROPERTY POSITION_INDEPENDENT_CODE TRUE)
set_property(TARGET fetcher PROPERTY EXCLUDE_FROM_ALL TRUE)

endif()
