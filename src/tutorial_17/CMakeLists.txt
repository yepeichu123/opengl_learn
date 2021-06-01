
project(directionalLight)

include_directories(
    # /usr/include/freetype2
    ${PROJECT_SOURCE_DIR}/include
    # ${PROJECT_SOURCE_DIR}/../../Common/FreetypeGL
)

# add_library(FreetypeGL SHARED
#     ${PROJECT_SOURCE_DIR}/../../Common/FreetypeGL/freetypeGL.cpp
#     ${PROJECT_SOURCE_DIR}/../../Common/FreetypeGL/font_shader.cpp
#     ${PROJECT_SOURCE_DIR}/../../Common/FreetypeGL/font-manager.c
#     ${PROJECT_SOURCE_DIR}/../../Common/FreetypeGL/texture-atlas.c
#     ${PROJECT_SOURCE_DIR}/../../Common/FreetypeGL/texture-font.c
#     ${PROJECT_SOURCE_DIR}/../../Common/FreetypeGL/texture-glyph.c
#     ${PROJECT_SOURCE_DIR}/../../Common/FreetypeGL/vector.c
#     ${PROJECT_SOURCE_DIR}/../../Common/FreetypeGL/vertex-buffer.c)

# target_link_libraries(FreetypeGL 
#     freetype
#     ${OPENGL_LIBRARY}
#     -lglut
#     -lGLEW
# )

add_executable(directionalLight
    ${PROJECT_SOURCE_DIR}/src/directionalLight.cpp
    ${PROJECT_SOURCE_DIR}/src/lighting_technique.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/math_3d.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/pipeline.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/camera.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_atb.cpp
    # ${PROJECT_SOURCE_DIR}/../../Common/ogldev_app.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/glut_backend.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_texture.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/technique.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_basic_lighting.cpp)
    
target_link_libraries(
    directionalLight 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW 
    AntTweakBar
    Magick++-6.Q16
    # FreetypeGL
)