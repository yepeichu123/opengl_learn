
project(cameraControl_2)

add_executable(cameraControl_2
    ${PROJECT_SOURCE_DIR}/cameraControl_2.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/math_3d.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/pipeline.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/camera.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_atb.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/glut_backend.cpp)
    
target_link_libraries(
    cameraControl_2 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW 
    AntTweakBar
)