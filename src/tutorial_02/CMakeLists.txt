
add_executable(helloDot helloDot.cpp)
target_link_libraries(
    helloDot 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW
)