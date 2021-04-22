#include <iostream>
#include <cstdio>
#include <GL/glew.h>
#include <GL/glut.h>
#include "ogldev_math_3d.h"
using namespace std;

GLuint VBO;

// rendering callback function
static void RenderSceneCB() {
    // clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // start vertice attribution
    glEnableVertexAttribArray(0);
    
    // bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // how to analyse data in buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // draw point 
    glDrawArrays(GL_POINTS, 0, 1);

    // prohibit vertice data 
    glDisableVertexAttribArray(0);

    // swap buffer
    glutSwapBuffers();
}

// create vertice buffer
static void CreateVertexBuffer() {
    // create vertice array
    Vector3f Vertices[1];
    Vertices[0] = Vector3f(0.0f, 0.0f, 0.0f);

    // create buffer
    glGenBuffers(1, &VBO);

    // bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // bind vertice
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

int main(int argc, char** argv) {
    // initialize glut
    glutInit(&argc, argv);

    // display mode 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // window setting
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("helloDot");

    // start rendering
    glutDisplayFunc(RenderSceneCB);

    // initialize glew
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }
   
    // clear screen with black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // create vertice buffer
    CreateVertexBuffer();

    // start inner loop
    glutMainLoop();

    return 0;
}