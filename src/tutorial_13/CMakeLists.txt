
project(cameraCoordinate)


add_executable(cameraCoordinate 
    ${PROJECT_SOURCE_DIR}/cameraCoordinate.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/math_3d.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/pipeline.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/camera.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_atb.cpp)


target_link_libraries(
    cameraCoordinate 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW
    AntTweakBar
)