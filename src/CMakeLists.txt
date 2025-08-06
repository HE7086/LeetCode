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

add_subdirectory(solution)

#==================== fetcher ====================

if(DEFINED ENV{GITHUB_ACTIONS})
# do not build fetcher in ci
return()
endif()

find_package(nlohmann_json)
if(NOT nlohmann_json_FOUND)
  FetchContent_Declare(json SYSTEM
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
  )
  FetchContent_MakeAvailable(json)
endif()

find_package(cpr)
if(NOT cpr_FOUND)
  FetchContent_Declare(cpr SYSTEM
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.11.2
  )
  FetchContent_MakeAvailable(cpr)
endif()

add_subdirectory(fetcher)

#==================== utils ====================

add_subdirectory(utils)
