#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// #include "ogldev_app.h"
#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"
#include "ogldev_texture.h"
#include "ogldev_glut_backend.h"
#include "ogldev_basic_lighting.h"
#include "mesh.h"

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1200

using namespace std;

const char* pVSFileName     = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_22/shader/shader.vs";
const char* pFSFileName     = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_22/shader/shader.fs";
const char* pTextureModel   = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_22/model/quad.obj";
const char* pTextureFile    = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_22/model/phoenix_ugv.md2";

static const float FieldDepth = 10.0f;


class loadModel : public ICallbacks//, public OgldevApp
{
public:
    // 初始化各项参数,包括光照等参数,观测窗口等
    loadModel()
    {
        m_pGameCamera = NULL;
        m_pEffect = NULL;
        m_scale = 0.0f;
        // 光源颜色
        m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
        // 光源幅值
        m_directionalLight.AmbientIntensity = 1.0f;
        // 漫反射强度
        m_directionalLight.DiffuseIntensity = 0.01f;
        // 光源方向
        m_directionalLight.Direction = Vector3f(1.0f, -1.0, 0.0);
        // 设置视锥参数
        m_persProjInfo.FOV = 60.0f;
        m_persProjInfo.Height = WINDOW_HEIGHT;
        m_persProjInfo.Width = WINDOW_WIDTH;
        m_persProjInfo.zNear = 1.0f;
        m_persProjInfo.zFar = 50.0f;              
    }

    // 析构三项对象
    ~loadModel()
    {
        delete m_pEffect;
        delete m_pGameCamera;
        delete m_pMesh;
    }

    bool Init()
    {
        // 设置相机摆放参数
        Vector3f Pos(3.0f, 7.0f, -10.0f);
        Vector3f Target(0.0f, -0.2f, 1.0f);
        Vector3f Up(0.0, 1.0f, 0.0f);

        // 创建相机对象
        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);
      
        // 创建光照对象
        m_pEffect = new BasicLightingTechnique();

        // 初始化包括添加,编译,链接着色器
        // 关联uniform变量
        if (!m_pEffect->Init(pVSFileName, pFSFileName)) {
            printf("Error initializing the lighting technique\n");
            return false;
        }

        // 启动渲染程序
        m_pEffect->Enable();

        // 设置纹理单元
        m_pEffect->SetColorTextureUnit(0);

        // 创建mesh对象
        m_pMesh = new Mesh();

        // 加载指定路径的3D模型
        return m_pMesh->LoadMesh(pTextureFile);
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

        // 清空深度/颜色缓冲区
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 设置点光源的参数
        m_scale += 0.01f;
        PointLight pl[2];
        pl[0].DiffuseIntensity = 0.25f;
        pl[0].Color = Vector3f(1.0f, 0.5f, 0.0f);
        pl[0].Position = Vector3f(3.0f, 1.0f, FieldDepth * (cosf(m_scale) + 1.0f) / 2.0f);
        pl[0].Attenuation.Linear = 0.1f;
        pl[1].DiffuseIntensity = 0.25f;
        pl[1].Color = Vector3f(0.0f, 0.5f, 1.0f);
        pl[1].Position = Vector3f(7.0f, 1.0f, FieldDepth * (sinf(m_scale) + 1.0f) / 2.0f);
        pl[1].Attenuation.Linear = 0.1f;
        m_pEffect->SetPointLights(2, pl);

        // 设置聚光灯的参数
        SpotLight sl;
        sl.DiffuseIntensity = 0.9f;
        sl.Color = Vector3f(0.0f, 1.0f, 1.0f);
        sl.Position = m_pGameCamera->GetPos();
        sl.Direction = m_pGameCamera->GetTarget();
        sl.Attenuation.Linear = 0.1f;
        sl.Cutoff = 10.0f;
        m_pEffect->SetSpotLights(1, &sl);

        // 设置相机姿态和投影矩阵
        Pipeline p;
        p.Scale(0.1f, 0.1f, 0.1f);
        p.Rotate(0.0f, m_scale, 0.0f);
        p.WorldPos(0.0f, 0.0f, 10.0f);
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
        p.SetPerspectiveProj(m_persProjInfo);

        // 设置光源位置和方向
        // 设置观测位置
        m_pEffect->SetWVP(p.GetWVPTrans());
        m_pEffect->SetWorldMatrix(p.GetWorldTrans());
        m_pEffect->SetDirectionalLight(m_directionalLight);
        m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
        m_pEffect->SetMatSpecularIntensity(0.0f);
        m_pEffect->SetMatSpecularPower(0);

        // 渲染网格
        m_pMesh->Render();

        // 刷新缓冲区
        glutSwapBuffers();
    }

    // 键盘选择操作
    void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE State)
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
        default:
            m_pGameCamera->OnKeyboard(OgldevKey);				
        }
    }

    // 鼠标选择金字塔位置
    virtual void PassiveMouseCB(int x, int y)
    {
        m_pGameCamera->OnMouse(x, y);
    }

private:

    BasicLightingTechnique* m_pEffect;
    DirectionalLight m_directionalLight;
    PersProjInfo m_persProjInfo;
    Camera* m_pGameCamera;
    float m_scale;
    Mesh* m_pMesh;
};


int main(int argc, char** argv)
{
    // 必须启动深度测试,即第三个参数
    // open GL_DEPTH_TEST
    GLUTBackendInit(argc, argv, true, false);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "loadModel")) {
        return 1;
    }

    loadModel* pApp = new loadModel();

    if (!pApp->Init()) {
        return 1;
    }

    pApp->Run();

    delete pApp;
 
    return 0;
}
