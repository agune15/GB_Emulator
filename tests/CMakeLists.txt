# Show CMake commands
set(CMAKE_VERBOSE_MAKEFILE ON)

# List src files
file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.c")
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/main.c")

# Removed src files included in tests
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/display.c")
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/interrupts.c")
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/timer.c")
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/memory.c")
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/input.c")
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/cpu.c")

# List test files
file(GLOB tests "${PROJECT_SOURCE_DIR}/tests/*.cpp")

add_executable(tests_run ${tests} ${sources})

# Include headers
target_include_directories(tests_run PRIVATE ${PROJECT_SOURCE_DIR}/include)