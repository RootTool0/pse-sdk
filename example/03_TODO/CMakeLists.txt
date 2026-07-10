cmake_minimum_required(VERSION 3.18)
project(03_TODO LANGUAGES CXX)

add_executable(03_TODO main.cpp)

target_include_directories(03_TODO PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/include")
target_link_directories(03_TODO PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/lib" "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/bin")
target_link_libraries(03_TODO PRIVATE pse)

add_custom_command(TARGET 03_TODO POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/bin/pse.dll"
    $<TARGET_FILE_DIR:03_TODO>
)
