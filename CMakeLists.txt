cmake_minimum_required(VERSION 3.0)
project(LearnOpenGLStepByStep)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_BUILD_TYPE "Release")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/Include
)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)