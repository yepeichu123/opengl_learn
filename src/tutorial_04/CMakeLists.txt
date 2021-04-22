
project(shadeTriangle)

add_executable(shadeTriangle 
    ${PROJECT_SOURCE_DIR}/shadeTriangle.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp)
target_link_libraries(
    shadeTriangle 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW
)