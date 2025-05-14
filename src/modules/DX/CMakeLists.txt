# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/third_party/DirectXTK/Inc)
include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/um")

# 编译 DX 静态库
add_library(DX STATIC
        DeviceResources.cpp
        Game.cpp
        pch.cpp
)

target_precompile_headers(DX PRIVATE pch.h)

target_link_libraries(DX PRIVATE DirectXTK)

# 编译 DXDemo 可执行程序
add_executable(DXDemo
        Main.cpp
)

target_link_libraries(DXDemo
        DX
        d3d11
        dxgi
        dxguid
        uuid
)

add_definitions(-DUNICODE -D_UNICODE)

if (MSVC)
    set_target_properties(DXDemo PROPERTIES
            LINK_FLAGS "/SUBSYSTEM:WINDOWS"
    )
endif()
