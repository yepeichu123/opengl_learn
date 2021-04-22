#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "ogldev_math_3d.h"
using namespace std;

GLuint VBO;

// rendering callback function
static void RenderSceneCB() {
    // clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // open vertice attributes
    glEnableVertexAttribArray(0);
    // bind GL_ARRAY_BUFFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // tell pipeline how to analyse buffers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // start drawing
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // close vertice attributes
    glDisableVertexAttribArray(0);

    // swap buffers
    glutSwapBuffers();

    // update
    // glFlush();
}

// create vertex buffer
static void CreateVertexBuffer() {
    // create 3 vertices to construct triangle
    Vector3f Vertices[3];
    Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
    Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
    Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);

    // create buffer
    glGenBuffers(1, &VBO);
    // bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // bind vertex data
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

int main(int argc, char** argv) {

    // initialize GLUT
    glutInit(&argc, argv);

    // display mode
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // window setting
    glutInitWindowSize(500, 500);
    // window position
    glutInitWindowPosition(100, 100);
    // window name
    glutCreateWindow("drawTriangle");

    // start to render
    glutDisplayFunc(RenderSceneCB);

    // reset window color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // initialize glew
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        cout << "Failed to initialize glew." << endl;
        return 1;
    }

    // create vertice buffer
    CreateVertexBuffer();

    // main loop
    glutMainLoop();

    return 0;
}