
project(shadowMap_2)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/../../Include/assimp
    ${PROJECT_SOURCE_DIR}/../../Include/assimp/Compiler
)

add_executable(shadowMap_2
    ${PROJECT_SOURCE_DIR}/src/shadowMap_2.cpp
    ${PROJECT_SOURCE_DIR}/src/mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/shadow_map_fbo.cpp
    ${PROJECT_SOURCE_DIR}/src/shadow_map_technique.cpp 
    ${PROJECT_SOURCE_DIR}/src/lighting_technique.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/math_3d.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/pipeline.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/camera.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_atb.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/glut_backend.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_texture.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/technique.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_basic_lighting.cpp)
    
target_link_libraries(
    shadowMap_2 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW 
    AntTweakBar
    Magick++-6.Q16
    assimp
)