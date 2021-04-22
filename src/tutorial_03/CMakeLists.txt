
project(drawTriangular)

add_executable(drawTriangular ${PROJECT_SOURCE_DIR}/drawTriangular.cpp)
target_link_libraries(
    drawTriangular 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW
)