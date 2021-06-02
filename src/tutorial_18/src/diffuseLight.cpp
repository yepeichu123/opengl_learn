#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// #include "ogldev_app.h"
#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"
#include "ogldev_texture.h"
#include "lighting_technique.h"
#include "ogldev_glut_backend.h"

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1200

using namespace std;

const char* pVSFileName = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_18/shader/shader.vs";
const char* pFSFileName = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_18/shader/shader.fs";
string textureImageFile = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_18/data/test.png";

struct Vertex
{
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    Vertex() {}

    Vertex(Vector3f pos, Vector2f tex)
    {
        m_pos = pos;
        m_tex = tex;
        m_normal = Vector3f(0.0f, 0.0f, 0.0f);
    }
};


class diffuseLight : public ICallbacks//, public OgldevApp
{
public:
    // 初始化各项参数,包括光照等参数,观测窗口等
    diffuseLight()
    {
        m_pGameCamera = NULL;
        m_pTexture = NULL;
        m_pEffect = NULL;
        m_scale = 0.0f;
        m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
        m_directionalLight.AmbientIntensity = 0.01f;
        m_directionalLight.DiffuseIntensity = 0.75f;
        m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);
        
        // 视锥
        m_persProjInfo.FOV = 60.0f;
        m_persProjInfo.Height = WINDOW_HEIGHT;
        m_persProjInfo.Width = WINDOW_WIDTH;
        m_persProjInfo.zNear = 1.0f;
        m_persProjInfo.zFar = 100.0f;                
    }

    // 析构三项对象
    ~diffuseLight()
    {
        delete m_pEffect;
        delete m_pGameCamera;
        delete m_pTexture;
    }

    bool Init()
    {
        Vector3f Pos(0.0f, 0.0f, -3.0f);
        Vector3f Target(0.0f, 0.0f, 1.0f);
        Vector3f Up(0.0, 1.0f, 0.0f);
        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

        unsigned int Indices[] = { 0, 3, 1,
                                   1, 3, 2,
                                   2, 3, 0,
                                   1, 2, 0 };

        // 创建顶点和指定索引渲染
        // 重载创建顶点和索引缓冲器创建函数，可以自主修改索引数组
        CreateIndexBuffer(Indices, sizeof(Indices));
        CreateVertexBuffer(Indices, ARRAY_SIZE_IN_ELEMENTS(Indices));

        m_pEffect = new LightingTechnique();

        // 初始化光照等参数,包括添加着色器和获取uniform变量
        if (!m_pEffect->Init(pVSFileName, pFSFileName))
        {
            printf("Error initializing the lighting technique\n");
            return false;
        }

        // 启动程序
        m_pEffect->Enable();

        // 将指定纹理单元关联到uniform变量
        m_pEffect->SetTextureUnit(0);

        // 创建纹理对象
        m_pTexture = new Texture(GL_TEXTURE_2D, textureImageFile.c_str());

        // 确认是否成功读取纹理图像
        if (!m_pTexture->Load()) {
            return false;
        }

        return true;
    }

    // 设置不渲染隐藏面,循环调用回调函数
    void Run()
    {
        // 将自身作为参数传入,在该函数中调用自身的多个回调函数
        GLUTBackendRun(this);
    }


    virtual void RenderSceneCB()
    {
        m_pGameCamera->OnRender();

        glClear(GL_COLOR_BUFFER_BIT);
       
        m_scale += 0.01f;

        Pipeline p;
        // 设置相机变换
        p.Rotate(0.0f, m_scale, 0.0f);
        p.WorldPos(0.0f, 0.0f, 3.0f);
        // 设置观测参数
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
        p.SetPerspectiveProj(m_persProjInfo);
        // 设置光照位置
        m_pEffect->SetWVP(p.GetWVPTrans());
        
        // 设置世界坐标系变换矩阵
        const Matrix4f& WorldTransformation = p.GetWorldTrans();
        m_pEffect->SetWorldMatrix(WorldTransformation);
        // 设置光照信息
        m_pEffect->SetDirectionalLight(m_directionalLight);
        
        // 启动顶点属性编辑
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        // 绑定数组缓冲器
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        // 设置顶点属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
        // 绑定数组缓冲器
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        // 绑定纹理
        m_pTexture->Bind(GL_TEXTURE0);
        // 根据指定索引绘制
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        // 关闭顶点属性编辑
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        // 缓冲区交换/刷新
        glutSwapBuffers();
    }

    // 键盘选择操作
    virtual void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE State)
    {
        switch (OgldevKey) {
            case OGLDEV_KEY_ESCAPE:
            case OGLDEV_KEY_q:
                    GLUTBackendLeaveMainLoop();
                    break;

            case OGLDEV_KEY_a:
                m_directionalLight.AmbientIntensity += 0.05f;
                break;

            case OGLDEV_KEY_s:
                m_directionalLight.AmbientIntensity -= 0.05f;
                break;

            case OGLDEV_KEY_z:
                m_directionalLight.DiffuseIntensity += 0.05f;
                break;

            case OGLDEV_KEY_x:
                m_directionalLight.DiffuseIntensity -= 0.05f;
                break;
        }
    }

    // 鼠标选择金字塔位置
    virtual void PassiveMouseCB(int x, int y)
    {
        m_pGameCamera->OnMouse(x, y);
    }

private:

    // 计算顶点法向量
    void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount,
                     Vertex* pVertices, unsigned int VertexCount)
    {
        // Accumulate each triangle normal into each of the triangle vertices
        for (unsigned int i = 0 ; i < IndexCount ; i += 3) {
            // 计算每个顶点各个平面的法向量
            // 通过同一平面的两个向量叉积，得到垂直于该平面的法向量
            unsigned int Index0 = pIndices[i];
            unsigned int Index1 = pIndices[i + 1];
            unsigned int Index2 = pIndices[i + 2];
            Vector3f v1 = pVertices[Index1].m_pos - pVertices[Index0].m_pos;
            Vector3f v2 = pVertices[Index2].m_pos - pVertices[Index0].m_pos;
            Vector3f Normal = v1.Cross(v2);
            Normal.Normalize();

            // 累加所有法向量
            pVertices[Index0].m_normal += Normal;
            pVertices[Index1].m_normal += Normal;
            pVertices[Index2].m_normal += Normal;
        }

        // 加权平均
        // Normalize all the vertex normals
        for (unsigned int i = 0 ; i < VertexCount ; i++) {
            pVertices[i].m_normal.Normalize();
        }
    }

    // 创建顶点
    // 输入参数包括索引矩阵和索引矩阵长度
    // 在初始化顶点之后，计算法向量
    // 随后绑定顶点
    void CreateVertexBuffer(const unsigned int* pIndices, unsigned int IndexCount)
    {
        Vertex Vertices[4] = { Vertex(Vector3f(-1.0f, -1.0f, 0.5773f), Vector2f(0.0f, 0.0f)),
                               Vertex(Vector3f(0.0f, -1.0f, -1.15475f), Vector2f(0.5f, 0.0f)),
                               Vertex(Vector3f(1.0f, -1.0f, 0.5773f),  Vector2f(1.0f, 0.0f)),
                               Vertex(Vector3f(0.0f, 1.0f, 0.0f),      Vector2f(0.5f, 1.0f)) };
        
        unsigned int VertexCount = ARRAY_SIZE_IN_ELEMENTS(Vertices);

        CalcNormals(pIndices, IndexCount, Vertices, VertexCount);

        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    }

    // 创建索引缓冲器，通过指定索引的方式绘制三角形
    // 防止不断创建顶点对象，导致内存占用过多
    void CreateIndexBuffer(const unsigned int* pIndices, unsigned int SizeInBytes)
    {
        glGenBuffers(1, &m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeInBytes, pIndices, GL_STATIC_DRAW);
    }

    GLuint m_VBO;
    GLuint m_IBO;
    LightingTechnique* m_pEffect;
    Texture* m_pTexture;
    Camera* m_pGameCamera;
    float m_scale;
    DirectionalLight m_directionalLight;
    PersProjInfo m_persProjInfo;    
};


int main(int argc, char** argv)
{
//    Magick::InitializeMagick(*argv);
    GLUTBackendInit(argc, argv, false, false);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "diffuseLight")) {
        return 1;
    }

    diffuseLight* pApp = new diffuseLight();

    if (!pApp->Init()) {
        return 1;
    }

    pApp->Run();

    delete pApp;
 
    return 0;
}
