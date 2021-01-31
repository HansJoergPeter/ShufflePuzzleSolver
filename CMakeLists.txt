##############################################################################
cmake_minimum_required(VERSION 3.0)
project(SlidingPuzzleSolver VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT WIN32)
  add_compile_options(-Wall -Wextra)
endif()


##############################################################################
find_package(absl CONFIG)


##############################################################################
add_library(spsolver  INTERFACE)
target_include_directories(spsolver  INTERFACE  src/  ${CMAKE_BINARY_DIR}/)
if (TARGET absl::flat_hash_map)
  target_link_libraries(spsolver  INTERFACE  absl::flat_hash_map)
  set(HAS_ABSEIL  "1")
  message("Compiling +++WITH+++ support for Abseil")
else()
  set(HAS_ABSEIL  "0")
  message("Compiling ---WITHOUT--- support for Abseil")
endif()
configure_file(src/Config.h.in  ${CMAKE_BINARY_DIR}/Config.h)
add_library(SlidingPuzzleSolver::solver  ALIAS  spsolver)


##############################################################################
add_executable(solve_3x3  src/solve_3x3.cpp)
target_link_libraries(solve_3x3  PRIVATE
  SlidingPuzzleSolver::solver
)


##############################################################################
add_executable(solve_4x4  src/solve_4x4.cpp)
target_link_libraries(solve_4x4  PRIVATE
  SlidingPuzzleSolver::solver
)


##############################################################################
install(
  TARGETS
    solve_3x3
    solve_4x4
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
