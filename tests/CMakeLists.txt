# Show CMake commands
set(CMAKE_VERBOSE_MAKEFILE ON)

# Compile flags
#set(CMAKE_CXX_FLAGS_DEBUG "--unresolved-symbols=ignore-all")

# List src files
file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.c")
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/main.c")

# Removed src files included in tests
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/display.c")

# List test files
file(GLOB tests "${PROJECT_SOURCE_DIR}/tests/*.cpp")

add_executable(tests_run ${tests} ${sources})

# Include headers
target_include_directories(tests_run PRIVATE ${PROJECT_SOURCE_DIR}/include)