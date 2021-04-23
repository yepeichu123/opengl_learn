
project(movingTriangle)

add_executable(movingTriangle 
    ${PROJECT_SOURCE_DIR}/movingTriangle.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp)
target_link_libraries(
    movingTriangle 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW
)