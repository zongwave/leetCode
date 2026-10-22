cmake_minimum_required(VERSION 3.25)

project(cpp_playground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 本目录所有 .cpp 自动参与编译；CONFIGURE_DEPENDS 让增删文件后下次构建自动感知
file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(cpp_playground ${SOURCES})

if(MSVC)
  # /utf-8 源码编码；/EHsc 同步异常模型（try/catch 需要；VS 工程模板默认带，CMake 不会自动加）
  target_compile_options(cpp_playground PRIVATE /utf-8 /EHsc)
endif()
