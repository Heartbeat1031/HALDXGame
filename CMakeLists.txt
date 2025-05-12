# HALDXGame の CMake プロジェクト
cmake_minimum_required(VERSION 3.20)
project(HALDXGame)

# UTF-8 文字コードのサポート
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

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

# third_party ディレクトリの追加
add_subdirectory(third_party/JoltPhysics/Build JoltBuild)
add_subdirectory(third_party/DirectXTK)

#モジュールの追加
add_subdirectory(src/modules/GameFramework)
add_subdirectory(src/modules/Physics)

# HALDXGame のソースコード
add_executable(HALDXGame src/main.cpp)

# HALDXGame のインクルードディレクトリ
target_link_libraries(HALDXGame PRIVATE GameFramework)
target_link_libraries(HALDXGame PRIVATE Physics)