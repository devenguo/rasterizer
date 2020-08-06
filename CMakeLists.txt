cmake_minimum_required (VERSION 3.0)
project(rasterizer)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(Eigen3 REQUIRED)
set(EIGEN3_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/eigen)
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
# message(${EIGEN3_INCLUDE_DIR})

set(SRCS main.cpp
    ${PROJECT_SOURCE_DIR}/src/line.cpp
    ${PROJECT_SOURCE_DIR}/src/tgaimage.cpp
    ${PROJECT_SOURCE_DIR}/src/obj_reader.cpp
    ${PROJECT_SOURCE_DIR}/src/triangle.cpp
    ${PROJECT_SOURCE_DIR}/src/transform.cpp
    ${PROJECT_SOURCE_DIR}/src/rasterizer.cpp
    ${PROJECT_SOURCE_DIR}/src/shader.cpp
    ${PROJECT_SOURCE_DIR}/examples/ex1_draw_lines.cpp
    ${PROJECT_SOURCE_DIR}/examples/ex2_draw_triangles.cpp
    ${PROJECT_SOURCE_DIR}/examples/ex3_rasterizer.cpp)
add_executable(rasterizer ${SRCS})