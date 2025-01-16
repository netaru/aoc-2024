cmake_minimum_required(VERSION 3.14.0)
project(aoc_2024)

set(BUILD_TESTS OFF CACHE BOOL "Build with tests.")

set (CMAKE_CXX_STANDARD 23)

include_directories("include")

file(GLOB aoc_folders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} [0-9]*)
foreach(folder ${aoc_folders})
  add_executable(${folder} "${folder}/main.cc")
  add_custom_target("${folder}-run" COMMAND "${PROJECT_BINARY_DIR}/${folder}" < "${CMAKE_CURRENT_SOURCE_DIR}/${folder}/input.txt" DEPENDS ${folder})
  add_custom_target("${folder}-test" COMMAND "${PROJECT_BINARY_DIR}/${folder}" < "${CMAKE_CURRENT_SOURCE_DIR}/${folder}/test.txt" DEPENDS ${folder})
  if (LINUX)
    target_link_libraries(${folder} PRIVATE "-ltbb")
  endif()
endforeach(folder)

if(${BUILD_TESTS})
  enable_testing()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test test)
endif()
