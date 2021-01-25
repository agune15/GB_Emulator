# Show CMake commands
set(CMAKE_VERBOSE_MAKEFILE ON)

project(tests_run)

add_executable(tests_run test_main.cpp)

#target_include_directories(tests_run PRIVATE "C:/Users/alex.nebot/Desktop/GB_Emulator/src")
# target_link_libraries(tests_run "C:/Users/alex.nebot/Desktop/GB_Emulator/src")
# target_link_libraries(tests_run include)