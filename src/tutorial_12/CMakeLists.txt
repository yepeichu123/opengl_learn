
project(perspectivePyramid)

add_executable(perspectivePyramid 
    ${PROJECT_SOURCE_DIR}/perspectivePyramid.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/math_3d.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/pipeline.cpp)

target_link_libraries(
    perspectivePyramid 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW 
)