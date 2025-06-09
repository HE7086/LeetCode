include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/solution/*.cpp")
foreach(source_file ${SOURCE_FILES})
  get_filename_component(exe ${source_file} NAME_WE)
  string(REGEX MATCH "s([0-9]+)_.*" _ ${exe})
  add_executable(${exe} ${source_file})
  add_test(NAME "test_${CMAKE_MATCH_1}" COMMAND ${exe})
endforeach()
