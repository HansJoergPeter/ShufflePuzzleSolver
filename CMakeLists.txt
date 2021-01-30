cmake_minimum_required(VERSION 3.5)
project(PuzzleSolver VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(absl CONFIG REQUIRED)

if (NOT WIN32)
  add_compile_options(-Wall -Wextra -Wno-unused-variable -Wno-unused-parameter)
endif()

add_executable(puzzle  main.cpp)
target_link_libraries(puzzle PRIVATE
  absl::flat_hash_map
  absl::flat_hash_set
)
