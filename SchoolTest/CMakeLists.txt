cmake_minimum_required(VERSION 3.10)
project(SchoolTest)
set(CMAKE_CXX_STANDARD 20)

# 输出可执行文件和库到 build/bin 和 build/lib
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/SchoolTest)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/SchoolTest)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/SchoolTest)
endforeach()


file(GLOB_RECURSE TEST_SRC "*.cpp")
file(GLOB_RECURSE TEST_HEADERS "*.h")

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build Assimp tests" FORCE)

# 添加可执行文件，假设主程序是 main.cpp
add_executable(SchoolTest
        ${TEST_SRC}
        ${TEST_HEADERS}
)

target_link_libraries(SchoolTest PRIVATE DirectXTK)
target_link_libraries(SchoolTest PRIVATE DX)
