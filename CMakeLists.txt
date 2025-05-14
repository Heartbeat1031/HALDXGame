# HALDXGame の CMake プロジェクト
cmake_minimum_required(VERSION 3.20)
project(HALDXGame)

# UTF-8 文字コードのサポート
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
# 在 Windows 平台强制使用 UTF-8
if (WIN32)
  add_definitions(-DUNICODE -D_UNICODE)
endif()

# C++20 標準
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# directxmathConfig.cmake のチェックを回避するために、偽のパッケージパスを追加する
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/cmake/fake_directxmath" ${CMAKE_PREFIX_PATH})

#includeパスを追加する（Windows SDK ＋ DirectXTK）
include_directories(${CMAKE_SOURCE_DIR}/third_party/DirectXTK/Inc)
include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/um")

# third_party ディレクトリの追加
add_subdirectory(third_party/JoltPhysics/Build JoltBuild)
add_subdirectory(third_party/DirectXTK)

#モジュールの追加
add_subdirectory(src/modules/GameFramework)
add_subdirectory(src/modules/Physics)
add_subdirectory(src/modules/DX)

# HALDXGame のソースコード
add_executable(HALDXGame src/main.cpp)

# プリコンパイル済みヘッダーを追加する
target_precompile_headers(HALDXGame PRIVATE src/modules/DX/pch.h)

# DirectXTK のリンクを追加する
target_link_libraries(HALDXGame PRIVATE DirectXTK)

# HALDXGame のインクルードディレクトリ
target_link_libraries(HALDXGame PRIVATE GameFramework)
target_link_libraries(HALDXGame PRIVATE Physics)
target_link_libraries(HALDXGame PRIVATE DX)


#  リソースフォルダ「assets」をビルドディレクトリにコピーする
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
