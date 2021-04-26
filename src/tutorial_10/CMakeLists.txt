
project(indexDrawing)

add_executable(indexDrawing 
    ${PROJECT_SOURCE_DIR}/indexDrawing.cpp
    ${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp)
target_link_libraries(
    indexDrawing 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW
)