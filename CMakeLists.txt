cmake_minimum_required(VERSION 3.18)
project(pse LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source
add_library(pse SHARED
    "include/pse.h"
    "include/enums.h"
    "include/structures.h"
    "include/pse.hpp"
    "source/pse.cpp"
)

# Package
target_include_directories(pse 
    PRIVATE "source"
    PUBLIC "include"
)
target_compile_definitions(pse PRIVATE PSE_EXPORTS)

install(TARGETS pse
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES
    "include/pse.h"
    "include/enums.h"
    "include/structures.h"
    "include/pse.hpp"
	DESTINATION include/pse
)

# Call:
# mkdir build
# cd build
# cmake -DCMAKE_INSTALL_PREFIX="../dist" ..
