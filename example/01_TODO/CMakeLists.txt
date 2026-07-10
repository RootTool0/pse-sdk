cmake_minimum_required(VERSION 3.18)
project(01_TODO LANGUAGES C)

add_executable(01_TODO main.c)

target_include_directories(01_TODO PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/include")
target_link_directories(01_TODO PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/lib" "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/bin")
target_link_libraries(01_TODO PRIVATE pse)

add_custom_command(TARGET 01_TODO POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/bin/pse.dll"
    $<TARGET_FILE_DIR:01_TODO>
)
