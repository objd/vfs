file(GLOB_RECURSE VFS_TEST_FILES "t-**.hpp" "t-**.cpp")

add_executable(t-runner ${VFS_TEST_FILES})
set_target_properties(t-runner PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(t-runner gtest gmock vfs-uv vfs-unix uv)

add_test(NAME all-tests COMMAND t-runner)