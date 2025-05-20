# HALDXGame の CMake プロジェクト
cmake_minimum_required(VERSION 3.20)
project(HALDXGame)

if (WIN32)
  add_definitions(-DUNICODE -D_UNICODE)
endif()

# C++17 標準
set(CMAKE_CXX_STANDARD 17)
add_compile_definitions(UNICODE _UNICODE)
if (WIN7_SYSTEM_SUPPORT MATCHES ON)
  add_compile_definitions(_WIN32_WINNT=0x601)
endif()

# 编译器の設定
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# MSVC デバッグ情報の設定
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# CMake のバージョンを指定
set(BUILD_DIRECTXTK_WERROR OFF CACHE BOOL "" FORCE)
set(USE_DIRECTX_MATH TRUE CACHE BOOL "" FORCE)

file(GLOB HLSL_FILES shaders/*.hlsl shaders/*.hlsli)
foreach(HLSL_FILE ${HLSL_FILES})
  get_filename_component(HLSL_FDIR ${HLSL_FILE} DIRECTORY)
  get_filename_component(HLSL_FBASENAME_WE ${HLSL_FILE} NAME_WE)
  string(CONCAT HLSL_FNAME_WE ${HLSL_FDIR} / ${HLSL_FBASENAME_WE})

  string(LENGTH ${HLSL_FBASENAME_WE} LEN_FNAME_WE)
  math(EXPR LEN_FNAME_WE "${LEN_FNAME_WE}-2")
  string(SUBSTRING ${HLSL_FBASENAME_WE} ${LEN_FNAME_WE} 2 ENTRY_POINT)
  string(TOLOWER ${ENTRY_POINT} SHADER_TYPE)

  if ("${SHADER_TYPE}" STREQUAL "vs")
    set(SHADER_TYPE "Vertex")
  elseif("${SHADER_TYPE}" STREQUAL "hs")
    set(SHADER_TYPE "Hull")
  elseif("${SHADER_TYPE}" STREQUAL "ds")
    set(SHADER_TYPE "Domain")
  elseif("${SHADER_TYPE}" STREQUAL "gs")
    set(SHADER_TYPE "Geometry")
  elseif("${SHADER_TYPE}" STREQUAL "ps")
    set(SHADER_TYPE "Pixel")
  elseif("${SHADER_TYPE}" STREQUAL "cs")
    set(SHADER_TYPE "Compute")
  endif()
  set_source_files_properties(${HLSL_FILE} PROPERTIES
          VS_SHADER_OBJECT_FILE_NAME shaders/${HLSL_FBASENAME_WE}.cso
          VS_SHADER_TYPE ${SHADER_TYPE}
          VS_SHADER_MODEL 5.0
          VS_SHADER_ENTRYPOINT ${ENTRY_POINT}
          VS_SHADER_DISABLE_OPTIMIZATIONS $<$<CONFIG:Debug>:true>
          VS_SHADER_ENABLE_DEBUG $<$<CONFIG:Debug>:true>)
endforeach()

# 每次构建时都复制 assets 文件夹
set(ASSETS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/assets)
set(ASSETS_BINARY_DIR ${CMAKE_BINARY_DIR}/assets)

add_custom_command(
        OUTPUT ${ASSETS_BINARY_DIR}/.copied_stamp
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_SOURCE_DIR} ${ASSETS_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E touch ${ASSETS_BINARY_DIR}/.copied_stamp
        COMMENT "Copying asset files..."
        VERBATIM
)

add_custom_target(copy_assets ALL
        DEPENDS ${ASSETS_BINARY_DIR}/.copied_stamp
)

# third_party ディレクトリの追加
set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF CACHE BOOL "Use dynamic CRT for JoltPhysics" FORCE)
add_subdirectory(third_party/JoltPhysics/Build JoltBuild)
add_subdirectory(third_party/assimp assimp)
add_subdirectory(third_party/ImGui ImGui)

#モジュールの追加
add_subdirectory(src/modules/GameKit)
add_subdirectory(src/modules/GameFramework)
add_subdirectory(src/modules/Physics)
add_subdirectory(src/modules/DX DX)

file(GLOB_RECURSE BASE_CPP_FILES src/base/*.cpp)
file(GLOB_RECURSE BASE_HEADER_FILES src/base/*.h)
file(GLOB_RECURSE GAMEPLAY_CPP_FILES src/gameplay/*.cpp)
file(GLOB_RECURSE GAMEPLAY_HEADER_FILES src/gameplay/*.h)
source_group("base" FILES ${BASE_CPP_FILES} ${BASE_HEADER_FILES})
source_group("gameplay" FILES ${GAMEPLAY_CPP_FILES} ${GAMEPLAY_HEADER_FILES})

# HALDXGame のソースコード
add_executable(HALDXGame
        src/main.cpp
        ${HLSL_FILES}
        ${BASE_CPP_FILES}
        ${GAMEPLAY_CPP_FILES}
)
# UTF-8 文字コードのサポート
target_compile_options(HALDXGame PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# HALDXGame のインクルードディレクトリ
target_link_libraries(HALDXGame PRIVATE GameKit)
target_link_libraries(HALDXGame PRIVATE GameFramework)
target_link_libraries(HALDXGame PRIVATE Physics)
target_link_libraries(HALDXGame PRIVATE DX)

target_include_directories(HALDXGame PRIVATE
        ${CMAKE_SOURCE_DIR}/src/modules/DX
)
source_group("shaders" FILES ${HLSL_FILES})
add_dependencies(HALDXGame copy_assets)
set_target_properties(HALDXGame PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")