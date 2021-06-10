
project(skybox_render)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/../../Include/assimp
    ${PROJECT_SOURCE_DIR}/../../Include/assimp/Compiler
)

add_executable(skybox_render
    ${PROJECT_SOURCE_DIR}/src/skybox_render.cpp
    ${PROJECT_SOURCE_DIR}/src/mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/skybox_technique.cpp
    ${PROJECT_SOURCE_DIR}/src/skybox.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/math_3d.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/pipeline.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/camera.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_atb.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/glut_backend.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_texture.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/technique.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_basic_lighting.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/cubemap_texture.cpp)
    
target_link_libraries(
    skybox_render 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW 
    AntTweakBar
    Magick++-6.Q16
    assimp
)