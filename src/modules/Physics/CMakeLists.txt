add_library(Physics STATIC
        main.cpp
)

target_include_directories(Physics PUBLIC
        ${CMAKE_SOURCE_DIR}/third_party/JoltPhysics
)

target_link_libraries(Physics PUBLIC Jolt)

add_executable(PhysicsDemo
        Main.cpp
)

# 添加 include 路径（Jolt 的头文件）
target_include_directories(PhysicsDemo PRIVATE
        ${CMAKE_SOURCE_DIR}/third_party/JoltPhysics/Jolt
)

# 链接 Jolt 库（已经由 add_subdirectory 定义）
target_link_libraries(PhysicsDemo PRIVATE Jolt)