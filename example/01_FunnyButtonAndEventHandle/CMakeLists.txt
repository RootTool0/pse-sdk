cmake_minimum_required(VERSION 3.18)
project(01_FunnyButtonAndEventHandle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_FunnyButtonAndEventHandle main.cpp)

target_include_directories(01_FunnyButtonAndEventHandle PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/include")
target_link_directories(01_FunnyButtonAndEventHandle PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/lib" "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/bin")
target_link_libraries(01_FunnyButtonAndEventHandle PRIVATE pse)

add_custom_command(TARGET 01_FunnyButtonAndEventHandle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/../../dist/bin/pse.dll"
    $<TARGET_FILE_DIR:01_FunnyButtonAndEventHandle>
)
