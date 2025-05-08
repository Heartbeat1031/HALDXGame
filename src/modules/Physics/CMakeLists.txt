add_library(Physics STATIC
        main.cpp
)

target_include_directories(Physics PUBLIC
        ${CMAKE_SOURCE_DIR}/third_party/JoltPhysics/Jolt
)

target_link_libraries(Physics PUBLIC Jolt)

add_executable(PhysicsDemo
        main.cpp
)

target_include_directories(PhysicsDemo PRIVATE
        ${CMAKE_SOURCE_DIR}/third_party/JoltPhysics/Jolt
)

target_link_libraries(PhysicsDemo PRIVATE Physics Jolt)

set_target_properties(PhysicsDemo PROPERTIES INTERPROCEDURAL_OPTIMIZATION FALSE)