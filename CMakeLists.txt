cmake_minimum_required(VERSION 3.18)
project(PortalSolverEditorSDK)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "Source/*")
add_executable(PortalSolverEditorSDK ${SOURCE_FILES})

# Call:
# mkdir Build
# cd Build
# cmake ..
