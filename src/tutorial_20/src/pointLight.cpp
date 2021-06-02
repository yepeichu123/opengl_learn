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

const char* pVSFileName = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_20/shader/shader.vs";
const char* pFSFileName = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_20/shader/shader.fs";
const char* textureImageFile = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_20/data/test.png";

static const float FieldDepth = 20.0f;
static const float FieldWidth = 10.0f;

struct Vertex
{
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    Vertex() {}

    Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
    {
        m_pos    = pos;
        m_tex    = tex;
        m_normal = normal;
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
        m_directionalLight.AmbientIntensity = 0.0f;
        m_directionalLight.DiffuseIntensity = 0.01f;
        m_directionalLight.Direction = Vector3f(1.0f, -1.0, 0.0);
        
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
        Vector3f Pos(5.0f, 1.0f, -3.0f);
        Vector3f Target(0.0f, 0.0f, 1.0f);
        Vector3f Up(0.0, 1.0f, 0.0f);
        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

        CreateVertexBuffer();

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
        m_pTexture = new Texture(GL_TEXTURE_2D, textureImageFile);

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
       
        m_scale += 0.0057f;

        PointLight pl[2];
        pl[0].DiffuseIntensity = 0.5f;
        pl[0].Color = Vector3f(1.0f, 0.5f, 0.0f);
        pl[0].Position = Vector3f(3.0f, 1.0f, FieldDepth * (cosf(m_scale) + 1.0f) / 2.0f);
        pl[0].Attenuation.Linear = 0.1f;

        pl[1].DiffuseIntensity = 0.5f;
        pl[1].Color = Vector3f(0.0f, 0.5f, 1.0f);
        pl[1].Position = Vector3f(7.0f, 1.0f, FieldDepth * (sinf(m_scale) + 1.0f) / 2.0f);
        pl[1].Attenuation.Linear = 0.1f;
        m_pEffect->SetPointLights(2, pl);

        Pipeline p;
        // 设置相机变换
        p.WorldPos(0.0f, 0.0f, 1.0f);
        // 设置观测参数
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
        p.SetPerspectiveProj(m_persProjInfo);
        // 设置光照位置
        m_pEffect->SetWVP(p.GetWVPTrans());
        
        // 漫反射光相关
        // 设置世界坐标系变换矩阵
        const Matrix4f& WorldTransformation = p.GetWorldTrans();
        m_pEffect->SetWorldMatrix(WorldTransformation);
        // 设置光照信息
        m_pEffect->SetDirectionalLight(m_directionalLight);
        
        // 镜面反射光相关
        m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
        m_pEffect->SetMatSpecularIntensity(0.0f);
        m_pEffect->SetMatSpecularPower(0);

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
        // 绘制图形
        glDrawArrays(GL_TRIANGLES, 0, 6);

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

    void CreateVertexBuffer()
    {
        const Vector3f Normal = Vector3f(0.0, 1.0f, 0.0f);

        Vertex Vertices[6] = {
                                Vertex(Vector3f(0.0f, 0.0f, 0.0f),             Vector2f(0.0f, 0.0f), Normal),
                                Vertex(Vector3f(0.0f, 0.0f, FieldDepth),       Vector2f(0.0f, 1.0f), Normal),
                                Vertex(Vector3f(FieldWidth, 0.0f, 0.0f),       Vector2f(1.0f, 0.0f), Normal),

                                Vertex(Vector3f(FieldWidth, 0.0f, 0.0f),       Vector2f(1.0f, 0.0f), Normal),
                                Vertex(Vector3f(0.0f, 0.0f, FieldDepth),       Vector2f(0.0f, 1.0f), Normal),
                                Vertex(Vector3f(FieldWidth, 0.0f, FieldDepth), Vector2f(1.0f, 1.0f), Normal)
                             };

        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
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

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "pointLight")) {
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
