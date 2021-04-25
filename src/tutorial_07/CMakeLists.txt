
project(rotatingTriangle)

add_executable(rotatingTriangle 
    ${PROJECT_SOURCE_DIR}/rotatingTriangle.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp)
target_link_libraries(
    rotatingTriangle 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW
)