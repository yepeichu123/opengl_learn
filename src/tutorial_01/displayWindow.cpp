#include <iostream>
#include <GL/glut.h>

// rendering recall function
void RenderScenceCB() {
    // clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // change buffers
    glutSwapBuffers();
}

// main function
int main(int argc, char** argv) {
    // initialize GLUT
    glutInit(&argc, argv);

    // display mode
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // window setting
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(500, 100);
    glutCreateWindow("DisplayWindow");

    // start rendering
    glutDisplayFunc(RenderScenceCB);

    // reset color after clearing buffer
    glClearColor(0.0f, 1.0f, 1.0f, 0.0f);

    // start GLUT inner loop
    glutMainLoop();

    return 0;
}