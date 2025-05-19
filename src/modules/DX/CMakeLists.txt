
# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/third_party/DirectXTK/Inc)

# 编译 DX 静态库
file(GLOB DX_SRC
        "*.cpp"
)

add_library(DX STATIC ${DX_SRC})

target_compile_options(DX PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
# 添加 DirectXTK 头文件目录
target_link_libraries(DX
        PUBLIC
        d3d11
        dxgi
        dxguid
        uuid
)

target_precompile_headers(DX PRIVATE pch.h)

target_link_libraries(DX PUBLIC DirectXTK)
target_include_directories(DX PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(DX PUBLIC assimp)
target_link_libraries(DX PUBLIC ImGui)
