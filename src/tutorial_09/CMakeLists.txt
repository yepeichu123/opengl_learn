
project(interpolationTriangle)

add_executable(interpolationTriangle 
    ${PROJECT_SOURCE_DIR}/interpolationTriangle.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp)
target_link_libraries(
    interpolationTriangle 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW
)