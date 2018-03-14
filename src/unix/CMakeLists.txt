file(GLOB_RECURSE VFS_HEADER_FILES ${PROJECT_SOURCE_DIR}/include/**.hpp)
file(GLOB_RECURSE VFS_UNIX_FILES **.hpp **.cpp)

add_library(${PROJECT_NAME}-unix SHARED ${VFS_HEADER_FILES} ${VFS_UNIX_FILES})
set_target_properties(${PROJECT_NAME}-unix PROPERTIES LINKER_LANGUAGE CXX)
#target_link_libraries(${PROJECT_NAME}-unix uv)