
add_executable(displayWindow displayWindow.cpp)
target_link_libraries(
    displayWindow 
    ${OPENGL_LIBRARY}
    -lglut
)