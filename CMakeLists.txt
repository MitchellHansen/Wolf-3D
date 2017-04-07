# Check versions
message(STATUS "CMake version: ${CMAKE_VERSION}")
cmake_minimum_required(VERSION 3.1)

# Set the project name
set(PNAME Wolf-3D)
project(${PNAME})

# Set up variables, and find SFML
if (WIN32)
    set(SFML_ROOT root CACHE STRING "User specified path")
    set(SFML_INCLUDE_DIR ${SFML_ROOT}/include)
endif()

set(SFML_COMPONENTS graphics window system network audio)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(SFML 2.3 COMPONENTS ${SFML_COMPONENTS} REQUIRED)
message(STATUS "SFML found: ${SFML_FOUND}")

# Include the directories for SFML's headers
include_directories(${SFML_INCLUDE_DIR})
include_directories(include)

# Glob all thr sources into their values
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

add_executable(${PNAME} ${SOURCES} ${HEADERS} src/Camera.cpp include/Camera.h)

# Link CL, GL, and SFML
target_link_libraries (${PNAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

if (NOT WIN32)
	target_link_libraries (${PNAME} -lpthread)
endif()

# Setup to use C++14
set_property(TARGET ${PNAME} PROPERTY CXX_STANDARD 14)

