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

include(FetchContent)

FetchContent_Declare(
  Eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG master
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE)
set(EIGEN_BUILD_DOC OFF)
# note: To disable eigen tests,
# you should put this code in a add_subdirectory to avoid to change
# BUILD_TESTING for your own project too since variables are directory
# scoped
set(BUILD_TESTING OFF)
set(EIGEN_BUILD_PKGCONFIG OFF)
set( OFF)
FetchContent_MakeAvailable(Eigen)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

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

target_link_libraries(tower_exe PRIVATE tower_lib Eigen3::Eigen nlohmann_json::nlohmann_json)


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

target_compile_options(tower_exe -g -fsanitize=address,undefined)
