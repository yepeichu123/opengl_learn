
project(resizeTriangle)

add_executable(resizeTriangle 
    ${PROJECT_SOURCE_DIR}/resizeTriangle.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp)
target_link_libraries(
    resizeTriangle 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW
)