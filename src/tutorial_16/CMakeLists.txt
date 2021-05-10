
project(texture)

add_executable(texture
    ${PROJECT_SOURCE_DIR}/texture.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/math_3d.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/pipeline.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/camera.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_atb.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/glut_backend.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_texture.cpp)
    
target_link_libraries(
    texture 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW 
    AntTweakBar
)