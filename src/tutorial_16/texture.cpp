#include <iostream>
#include <string>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "ogldev_math_3d.h"
#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"
#include "ogldev_glut_backend.h"
#include "ogldev_texture.h"
using namespace std;

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

struct Vertex
{
    Vector3f m_pos;
    Vector2f m_tex;

    Vertex() {}

    Vertex(Vector3f pos, Vector2f tex)
    {
        m_pos = pos;
        m_tex = tex;
    }
};

// index buffer
GLuint IBO;
GLuint VBO;
GLuint gWVPLocation;
GLuint gSampler;
PersProjInfo gPersProjInfo;
Camera* pGameCamera = NULL;
Texture* pTexture = NULL;

const char* pVSFileName = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_16/shader/shader.vs";
const char* pFSFileName = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_16/shader/shader.fs";
string textureImageFile = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_16/data/test.png";

static void RenderSceneCB() {
    // clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // increasing value
    static float Scale = 0.0f;
    Scale += 0.01f;
    Pipeline p;
    p.Rotate(0.0f, Scale, 0.0f);
    p.WorldPos(0.0f, 0.0f, 3.0f);
    // camera transformation
    p.SetCamera(*pGameCamera);
    p.SetPerspectiveProj(gPersProjInfo);
    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
    
    pGameCamera->OnRender();

    // vertex attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);

    // Binding index buffer before drawing..
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    pTexture->Bind(GL_TEXTURE0);   

    // draw triangle
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    // close attributes
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glutSwapBuffers();
}

static void InitializeGlutCallbacks() {
    glutDisplayFunc(RenderSceneCB);
    // setting rendering function as global free callback
    glutIdleFunc(RenderSceneCB);
}

static void CreateVertexBuffer() {

    Vertex Vertices[4] = {
        Vertex(Vector3f(-1.0f, -1.0f, 0.5873f), Vector2f(0.0f, 0.0f)),
        Vertex(Vector3f(0.0f, -1.0f, -1.15475), Vector2f(0.5f, 0.0f)),
        Vertex(Vector3f(1.0f, -1.0f, 0.57773f), Vector2f(1.0f, 0.0f)),
        Vertex(Vector3f(0.0f, 1.0f, 0.0f), Vector2f(0.5f, 1.0f))
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer() {
    unsigned int Indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    // create buffer
    glGenBuffers(1, &IBO);
    // bind bufffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    // add buffer data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
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
    gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
    assert(gWVPLocation != 0xFFFFFFFF);
    gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
    assert(gSampler != 0xFFFFFFFF);
}


int main(int argc, char** argv) {

    // initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("texture");

    InitializeGlutCallbacks();

    pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);
    
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
    // create index buffer
    CreateIndexBuffer();

    // compile shader
    CompileShaders();

    // initial perspective transforma parameters
    gPersProjInfo.FOV = 60.0f;
    gPersProjInfo.Height = WINDOW_HEIGHT;
    gPersProjInfo.Width = WINDOW_WIDTH;
    gPersProjInfo.zNear = 1.0f;
    gPersProjInfo.zFar = 100.0f;

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glUniform1d(gSampler, 0);
    
    pTexture = new Texture(GL_TEXTURE_2D, textureImageFile.c_str());
    if (!pTexture->Load())
    {
        return 1;
    }

    // main loop
    glutMainLoop();


    return 0;
}