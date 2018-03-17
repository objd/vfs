# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download")
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download")

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src" "${CMAKE_BINARY_DIR}/googletest-build")

# The gtest/gmock targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")
endif ()

enable_testing()

file(GLOB_RECURSE VFS_CATCH_FILES ${PROJECT_SOURCE_DIR}/deps/catch/**.hpp)
file(GLOB_RECURSE VFS_TEST_FILES "t-**.hpp" "t-**.cpp")

add_executable(t-runner ${VFS_TEST_FILES} ${VFS_CATCH_FILES})
set_target_properties(t-runner PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(t-runner gmock gtest)

add_test(NAME unit-tests COMMAND t-runner)