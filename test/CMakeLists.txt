enable_testing()

file(GLOB_RECURSE VFS_TEST_FILES "test/t-**.cpp")

foreach (VFS_TEST_FILE ${VFS_TEST_FILES})
    get_filename_component(VFS_TEST ${VFS_TEST_FILE} NAME_WE)
    add_executable(${VFS_TEST} ${VFS_TEST_FILE})
    add_test(NAME ${VFS_TEST} COMMAND ${VFS_TEST_FILE})
endforeach ()