#include <iostream>
#include <string>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "ogldev_math_3d.h"
#include "ogldev_util.h"
using namespace std;

GLuint VBO;
GLuint gWorldLocation;

const char* pVSFileName = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_09/shader/shader.vs";
const char* pFSFileName = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_09/shader/shader.fs";

static void RenderSceneCB() {
    // clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // increasing value
    static float Scale = 0.0f;
    Scale += 0.0001f;
    Matrix4f World;
    World.m[0][0] = sinf(Scale); World.m[0][1] = 0.0f;         World.m[0][2] = 0.0f;         World.m[0][3] = 0.0f;
    World.m[1][0] = 0.0f;        World.m[1][1] = sinf(Scale);  World.m[1][2] = 0.0f;         World.m[1][3] = 0.0f;
    World.m[2][0] = 0.0f;        World.m[2][1] = 0.0f;         World.m[2][2] = sinf(Scale);  World.m[2][3] = 0.0f;
    World.m[3][0] = 0.0f;        World.m[3][1] = 0.0f;         World.m[3][2] = 0.0f;         World.m[3][3] = 1.0f;
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]);

    // vertex attributes
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // draw triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // close attributes
    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

static void InitializeGlutCallbacks() {
    glutDisplayFunc(RenderSceneCB);
    // setting rendering function as global free callback
    glutIdleFunc(RenderSceneCB);
}

static void CreateVertexBuffer() {
    Vector3f Vertices[3];
    Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f) * 0.5f;
    Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f) * 0.5f;
    Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f) * 0.5f;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType) {
    // define two shaders
    GLuint ShaderObj = glCreateShader(ShaderType);
    
    // check if success
    if (ShaderObj == 0) {
        cout << "Error createing shader type: " << ShaderType << endl;
        exit(0);
    }

    // define shader source 
    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0] = strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);

    // check shader error
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        cout << "Error compiling shader tyep: " << ShaderType << endl;
        exit(1);
    }

    // link shader
    glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders() {
    // create shader
    GLuint ShaderProgram = glCreateProgram();
    // check if success
    if (ShaderProgram == 0) {
        cout << "Error creating shader program." << endl;
        exit(1);
    }

    // store data buffer
    string vs, fs;
    if (!ReadFile(pVSFileName, vs)) {
        exit(1);
    }
    if (!ReadFile(pFSFileName, fs)) {
        exit(1);
    }

    // add vertex shader and fragment shader
    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    // link shader
    GLint success = 0;
    GLchar ErrorLog[1024] = {0};
    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        cout << "Error linking shader program: " << ErrorLog << endl;
        exit(1);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        std::cout << "Invalid shader program: " << ErrorLog << std::endl;
        exit(1);
    }

    glUseProgram(ShaderProgram);

    /// To get the uniform value location after linking program
    // check uniform value location
    gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
    // check error
    assert(gWorldLocation != 0xFFFFFFFF);
}


int main(int argc, char** argv) {

    // initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1025, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("interpolationTriangle");

    InitializeGlutCallbacks();

    // initialize glew
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        cout << "Failed to initialize glew.." << endl;
        return 1;
    }

    cout << "GL version: " << GL_VERSION << endl;

    // window color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // create vertices
    CreateVertexBuffer();

    // compile shader
    CompileShaders();

    // main loop
    glutMainLoop();


    return 0;
}