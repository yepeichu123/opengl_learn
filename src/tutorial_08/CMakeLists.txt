
project(scalingTriangle)

add_executable(scalingTriangle 
    ${PROJECT_SOURCE_DIR}/scalingTriangle.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp)
target_link_libraries(
    scalingTriangle 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW
)