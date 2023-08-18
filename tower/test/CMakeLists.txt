# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(towerTests LANGUAGES CXX)

# ---- Tests ----

add_executable(tower_test source/tower_test.cpp)
target_link_libraries(tower_test PRIVATE tower_lib)
target_compile_features(tower_test PRIVATE cxx_std_20)

add_test(NAME tower_test COMMAND tower_test)

# ---- End-of-file commands ----

add_folders(Test)
