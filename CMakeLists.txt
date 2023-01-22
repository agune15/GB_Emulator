# CMake minimum version
cmake_minimum_required(VERSION 3.17)

# Project name
project(GB_Emulator)

# C standard
set(CMAKE_C_STANDARD 11)

# Debug compile flags
set(CMAKE_C_FLAGS_DEBUG "-mconsole -DDEBUG")

# Debug compile flags
set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG")

# Add SDL2 CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Add all c source files under the src directory
file(GLOB sources "src/*.c")
add_executable(${PROJECT_NAME} ${sources})

# Add all headers files under the include directory
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Add SDL2 library
find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::Main)

# Add tests directory
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_subdirectory(tests)
endif()