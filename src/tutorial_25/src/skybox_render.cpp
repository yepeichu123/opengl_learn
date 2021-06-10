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
#include "mesh.h"
#include "ogldev_basic_lighting.h"
#include "skybox.h"
#include <string>

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1200

using namespace std;

const char* pVSFileName         = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_25/shader/shader.vs";
const char* pFSFileName         = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_25/shader/shader.fs";
const char* pVSSkyboxFileName   = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_25/shader/skybox_shader.vs";
const char* pFSSkyboxFileName   = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_25/shader/skybox_shader.fs";
const char* pTextureFile        = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_25/model/phoenix_ugv.md2";
const char* pTextureModel       = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_25/model/sphere.obj";
// 包围盒纹理路径和图像
const char* ModelDirectory      = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_25/model";
const char* PosXFilename        = "sp3right.jpg";
const char* NegXFilename        = "sp3left.jpg";
const char* PosYFilename        = "sp3top.jpg";
const char* NegYFilename        = "sp3bot.jpg";
const char* PosZFilename        = "sp3front.jpg";
const char* NegZFilename        = "sp3back.jpg";


class skyboxRender : public ICallbacks//, public OgldevApp
{
public:
    // 初始化各项参数,包括光照等参数,观测窗口等
    skyboxRender()
    {
        m_pLightingTechnique = NULL;        
        m_pGameCamera = NULL;        
        m_pTankMesh = NULL;
        m_scale = 0.0f;
        m_pSkyBox = NULL;

        // 环境光幅值
        m_dirLight.AmbientIntensity = 0.2f;
        // 漫反射强度
        m_dirLight.DiffuseIntensity = 0.8f;
        // 光源颜色
        m_dirLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
        // 光源方向
        m_dirLight.Direction = Vector3f(1.0f, -1.0f, 0.0f);
        
        // 视锥
        m_persProjInfo.FOV = 60.0f;
        m_persProjInfo.Height = WINDOW_HEIGHT;
        m_persProjInfo.Width = WINDOW_WIDTH;
        m_persProjInfo.zNear = 1.0f;
        m_persProjInfo.zFar = 100.0f;                
    }

    // 析构三项对象
    ~skyboxRender()
    {
        SAFE_DELETE(m_pLightingTechnique);
        SAFE_DELETE(m_pGameCamera);        
        SAFE_DELETE(m_pTankMesh);        
        SAFE_DELETE(m_pSkyBox);
    }

    bool Init()
    {
        // 设置相机摆放位置和姿态
        Vector3f Pos(0.0f, 1.0f, -20.0f);
        Vector3f Target(0.0f, 0.0f, 1.0f);
        Vector3f Up(0.0, 1.0f, 0.0f);
        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);
     
        m_pLightingTechnique = new BasicLightingTechnique();

        if (!m_pLightingTechnique->Init(pVSFileName, pFSFileName)) {
            printf("Error initializing the lighting technique\n");
            return false;
        }

        m_pLightingTechnique->Enable();
        m_pLightingTechnique->SetDirectionalLight(m_dirLight);
        m_pLightingTechnique->SetColorTextureUnit(0);
              
        // 加载三维坦克模型
        m_pTankMesh = new Mesh();
		if (!m_pTankMesh->LoadMesh(pTextureFile)) {
            return false;
        }
        
        // 加载包围盒纹理图像
        m_pSkyBox = new SkyBox(m_pGameCamera, m_persProjInfo);
		if (!m_pSkyBox->Init(
            pVSSkyboxFileName, pFSSkyboxFileName,
            pTextureModel, ModelDirectory,
			PosXFilename,NegXFilename,
			PosYFilename,NegYFilename,
			PosZFilename,NegZFilename)) {
            return false;
        }
        
        return true;
    }

    void Run()
    {
        // 将自身作为参数传入,在该函数中调用自身的多个回调函数
        GLUTBackendRun(this);
    }


    virtual void RenderSceneCB()
    {
        m_pGameCamera->OnRender();
        m_scale += 0.05f;
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_pLightingTechnique->Enable();
       
        Pipeline p;        
        p.Scale(0.1f, 0.1f, 0.1f);
        p.Rotate(0.0f, m_scale, 0.0f);
        p.WorldPos(0.0f, -5.0f, 3.0f);
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
        p.SetPerspectiveProj(m_persProjInfo);
        m_pLightingTechnique->SetWVP(p.GetWVPTrans());
        m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());
        // 渲染坦克纹理
        m_pTankMesh->Render();
        // 渲染包围盒内部纹理
        m_pSkyBox->Render();
        // 刷新缓冲区
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

    BasicLightingTechnique* m_pLightingTechnique;
    Camera* m_pGameCamera;
    float m_scale;
    DirectionalLight m_dirLight;    
    Mesh* m_pTankMesh;    
    SkyBox* m_pSkyBox;
    PersProjInfo m_persProjInfo;
};


int main(int argc, char** argv)
{
    // 第三个参数必须设置为true,表示开启深度测试，否则不会出现阴影
    GLUTBackendInit(argc, argv, true, false);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "skyboxRender")) {
        return 1;
    }

    skyboxRender* pApp = new skyboxRender();

    if (!pApp->Init()) {
        return 1;
    }

    pApp->Run();

    delete pApp;
 
    return 0;
}
