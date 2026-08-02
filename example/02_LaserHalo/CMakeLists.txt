cmake_minimum_required(VERSION 3.18)
project(02_LaserHalo LANGUAGES CXX)

add_executable(02_LaserHalo main.cpp)

target_include_directories(02_LaserHalo PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/include")
target_link_directories(02_LaserHalo PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/lib" "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/bin")
target_link_libraries(02_LaserHalo PRIVATE pse)

add_custom_command(TARGET 02_LaserHalo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/bin/pse.dll"
    $<TARGET_FILE_DIR:02_LaserHalo>
)
