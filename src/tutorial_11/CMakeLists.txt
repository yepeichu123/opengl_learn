
project(transformPyramid)

add_executable(transformPyramid 
    ${PROJECT_SOURCE_DIR}/transformPyramid.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/math_3d.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/pipeline.cpp)

target_link_libraries(
    transformPyramid 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW 
)