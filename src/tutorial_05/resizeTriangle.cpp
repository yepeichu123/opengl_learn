#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include "ogldev_math_3d.h"
#include "ogldev_util.h"
using namespace std;

GLuint VBO;
GLuint gScaleLocation;

const char* pVSFileName = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_05/shader/shader.vs";
const char* pFSFileName = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_05/shader/shader.fs";

static void RenderSceneCB() {
    // clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // increasing value
    static float Scale = 0.0f;
    Scale += 0.0001f;
    glUniform1f(gScaleLocation, sinf(Scale));

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
    Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
    Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
    Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);

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
    gScaleLocation = glGetUniformLocation(ShaderProgram, "gScale");
    // check error
    assert(gScaleLocation != 0xFFFFFFFF);
}


int main(int argc, char** argv) {

    // initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("resizeTriangle");

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