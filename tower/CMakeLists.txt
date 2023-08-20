cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    tower
    VERSION 0.1.0
    DESCRIPTION "a tower to reach the sky."
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

# ---- Declare library ----

add_library(
    tower_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    tower_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(tower_lib PUBLIC cxx_std_20)

# ---- Declare executable ----

add_executable(tower_exe source/main.cpp)
add_executable(tower::exe ALIAS tower_exe)

set_property(TARGET tower_exe PROPERTY OUTPUT_NAME tower)

target_compile_features(tower_exe PRIVATE cxx_std_20)

target_link_libraries(tower_exe PRIVATE tower_lib Eigen3::Eigen)


# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT tower_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of tower"
  )
endif()

include(cmake/dev-mode.cmake)
