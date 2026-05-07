cmake_minimum_required(VERSION 3.18)
project(PSE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source
file(GLOB_RECURSE FILES CONFIGURE_DEPENDS "Source/*")
add_library(PSE SHARED ${FILES})

target_include_directories(PSE PUBLIC "Source")
target_compile_definitions(PSE PRIVATE PSE_EXPORTS)

install(TARGETS PSE
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

install(FILES "Source/PSE.h" "Source/Enums.h" "Source/Structures.h" DESTINATION include/PSE)

# Call:
# mkdir Build
# cd Build
# cmake -DCMAKE_INSTALL_PREFIX="../Dist" ..
