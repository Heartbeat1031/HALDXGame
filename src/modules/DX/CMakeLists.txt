# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/third_party/DirectXTK/Inc)

# 编译 DX 静态库
add_library(DX STATIC .)

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