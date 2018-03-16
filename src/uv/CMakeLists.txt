file(GLOB_RECURSE VFS_HEADER_FILES ${PROJECT_SOURCE_DIR}/include/**.hpp)
file(GLOB_RECURSE VFS_UV_FILES **.hpp **.cpp)

add_library(${PROJECT_NAME}-uv SHARED ${VFS_HEADER_FILES} ${VFS_UV_FILES})
set_target_properties(${PROJECT_NAME}-uv PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME}-uv ${PROJECT_NAME}-unix uv)