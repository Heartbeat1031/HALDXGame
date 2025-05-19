cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
add_compile_definitions(UNICODE _UNICODE)
if (WIN7_SYSTEM_SUPPORT MATCHES ON)
    add_compile_definitions(_WIN32_WINNT=0x601)
endif()

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

aux_source_directory(. DIR_SRCS)
file(GLOB HEADER_FILES ./*.h)


add_library(DX ${DIR_SRCS} ${HEADER_FILES})
target_link_libraries(DX d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib winmm.lib)

# ImGui
target_link_libraries(DX ImGui)
file(COPY imgui.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Assimp
target_link_libraries(DX assimp)
set_target_properties(DX PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${ASSIMP_LIBRARY_OUTPUT_DIRECTORY}/$<IF:$<CONFIG:Debug>,Debug,Release>")

# Common
add_subdirectory(Common)
target_link_libraries(DX Common)

set_target_properties(DX PROPERTIES OUTPUT_NAME "DX")

target_include_directories(DX PUBLIC .)