set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE RubikCube_CPP_FILES *.cpp)
file(GLOB_RECURSE RubikCube_HEADER_FILES *.h)

add_executable(RubikCube
        main.cpp
        ${RubikCube_CPP_FILES}
        ${RubikCube_HEADER_FILES}
)

#target_compile_options(RubikCube PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# ImGui
target_link_libraries(RubikCube PRIVATE ImGui)
file(COPY imgui.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(RubikCube PRIVATE DirectXTK)

target_include_directories(RubikCube PRIVATE
        ${CMAKE_SOURCE_DIR}/third_party/DirectXTK/Inc
        ${CMAKE_SOURCE_DIR}/third_party/ImGui
)

target_include_directories(RubikCube PUBLIC .)