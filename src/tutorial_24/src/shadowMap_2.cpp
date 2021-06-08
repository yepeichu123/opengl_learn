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
#include "shadow_map_technique.h"
#include "mesh.h"
#include "shadow_map_fbo.h"
#include "lighting_technique.h"

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1200

using namespace std;

const char* pVSFileName           = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_24/shader/shader.vs";
const char* pFSFileName           = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_24/shader/shader.fs";
const char* pVSShadowFileName     = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_24/shader/shaderShadow.vs";
const char* pFSShadowFileName     = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_24/shader/shaderShadow.fs";
const char* pTextureModel         = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_24/model/quad.obj";
const char* pTextureFile          = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_24/model/phoenix_ugv.md2";
const char* pTestImage            = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_24/model/test.png";


class shadowMap : public ICallbacks//, public OgldevApp
{
public:
    // 初始化各项参数,包括光照等参数,观测窗口等
    shadowMap()
    {
        m_pGroundTex = NULL;
        m_pLightingEffect = NULL;
        m_pShadowMapEffect = NULL;
        m_pGameCamera = NULL;
        m_pMesh = NULL;
        m_pQuad = NULL;
        m_scale = 0.0f;

        // 环境光幅值
        m_spotLight.AmbientIntensity = 0.0f;
        // 漫反射强度
        m_spotLight.DiffuseIntensity = 0.9f;
        // 光源颜色
        m_spotLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
        // 线性衰减项
        m_spotLight.Attenuation.Linear = 0.01f;
        // 光源位置
        m_spotLight.Position  = Vector3f(-20.0, 20.0, 5.0f);
        // 光源方向
        m_spotLight.Direction = Vector3f(1.0f, -1.0f, 0.0f);
        // 光源视锥
        m_spotLight.Cutoff =  20.0f;
        
        // 视锥
        m_persProjInfo.FOV = 60.0f;
        m_persProjInfo.Height = WINDOW_HEIGHT;
        m_persProjInfo.Width = WINDOW_WIDTH;
        m_persProjInfo.zNear = 1.0f;
        m_persProjInfo.zFar = 50.0f;                
    }

    // 析构三项对象
    ~shadowMap()
    {
        SAFE_DELETE(m_pGroundTex);
        SAFE_DELETE(m_pLightingEffect);
        SAFE_DELETE(m_pShadowMapEffect);
        SAFE_DELETE(m_pGameCamera);
        SAFE_DELETE(m_pMesh);
        SAFE_DELETE(m_pQuad);
    }

    bool Init()
    {
        Vector3f Pos(3.0f, 8.0f, -10.0f);
        Vector3f Target(0.0f, -0.2f, 1.0f);
        Vector3f Up(0.0, 1.0f, 0.0f);

        if (!m_shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
            return false;
        }

        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);
     
        m_pLightingEffect = new LightingTechnique();
        // 创建着色器并关联uniform变量
        if (!m_pLightingEffect->Init(pVSFileName, pFSFileName)) {
            printf("Error initializing the shadow map technique\n");
            return false;
        }        
        m_pLightingEffect->Enable();
        m_pLightingEffect->SetSpotLights(1, &m_spotLight);
        m_pLightingEffect->SetTextureUnit(0);
        m_pLightingEffect->SetShadowMapTextureUnit(1);
       
        m_pShadowMapEffect = new ShadowMapTechnique();
        if (!m_pShadowMapEffect->Init(pVSShadowFileName, pFSShadowFileName)) {
            printf("Error initializing the shadow map technique\n");
            return false;
        }

        // 用于加载模型
        m_pQuad = new Mesh();
		if (!m_pQuad->LoadMesh(pTextureModel)) {
            return false;
        }

        // 用于渲染地面的纹理
        m_pGroundTex = new Texture(GL_TEXTURE_2D, pTestImage);
        if (!m_pGroundTex->Load()) {
            return false;
        }

        // 用于渲染模型本身的纹理
        m_pMesh = new Mesh();
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
        m_scale += 0.05f;

        // 渲染阴影贴图，得到深度信息
        ShadowMapPass();

        // 以纹理图像和深度信息作为输入，渲染图像
        RenderPass();
        
        // 刷新缓冲区
        glutSwapBuffers();
    }

    // 阴影贴图渲染
    // 将深度信息渲染到shadow map纹理中
    virtual void ShadowMapPass()
    {
        // 绑定FBO，将所有深度值渲染到shadow map中
        m_shadowMapFBO.BindForWriting();

        glClear(GL_DEPTH_BUFFER_BIT);

        // 使用阴影着色器
        m_pShadowMapEffect->Enable();

        // 设置相机位置和变换
        Pipeline p;
        p.Scale(0.1f, 0.1f, 0.1f);
        p.Rotate(0.0f, m_scale, 0.0f);
        p.WorldPos(0.0f, 0.0f, 3.0f);
        // 相机位置设置与光源在一位置
        p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
        p.SetPerspectiveProj(m_persProjInfo);
        m_pShadowMapEffect->SetWVP(p.GetWVPTrans());
        m_pMesh->Render();
        
        // 切换回默认的frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    // 将阴影贴图显示到模型上
    virtual void RenderPass()
    {
        // 清除颜色和深度缓冲区
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
        // 使用光照着色器代替阴影着色器
        m_pLightingEffect->Enable();

        m_pLightingEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

        // 绑定阴影贴图的FBO到纹理单元1号以供读取
        m_shadowMapFBO.BindForReading(GL_TEXTURE1);

        Pipeline p;
        p.SetPerspectiveProj(m_persProjInfo);

        // 渲染阴影到地面纹理中
        p.Scale(10.0f, 10.0f, 10.0f);
        p.WorldPos(0.0f, 0.0f, 1.0f);
        p.Rotate(90.0f, 0.0f, 0.0f);
        // 相机自身位置
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
        m_pLightingEffect->SetWVP(p.GetWVPTrans());
        m_pLightingEffect->SetWorldMatrix(p.GetWorldTrans());        
        p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
        m_pLightingEffect->SetLightWVP(p.GetWVPTrans());
        // 绑定一个纹理贴图
        m_pGroundTex->Bind(GL_TEXTURE0);
        m_pQuad->Render();

        // 渲染模型本身
        p.Scale(0.1f, 0.1f, 0.1f);
        p.Rotate(0.0f, m_scale, 0.0f);
        p.WorldPos(0.0f, 0.0f, 3.0f);
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
        m_pLightingEffect->SetWVP(p.GetWVPTrans());
        m_pLightingEffect->SetWorldMatrix(p.GetWorldTrans());
        p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
        m_pLightingEffect->SetLightWVP(p.GetWVPTrans());
        m_pMesh->Render();
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

    LightingTechnique* m_pLightingEffect;
    ShadowMapTechnique* m_pShadowMapEffect;
    Camera* m_pGameCamera;
    float m_scale;
    SpotLight m_spotLight;
    Mesh* m_pMesh;
    Mesh* m_pQuad;
    Texture* m_pGroundTex;
    ShadowMapFBO m_shadowMapFBO;
    PersProjInfo m_persProjInfo;	
};


int main(int argc, char** argv)
{
    // 第三个参数必须设置为true,表示开启深度测试，否则不会出现阴影
    GLUTBackendInit(argc, argv, true, false);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "shadowMap_2")) {
        return 1;
    }

    shadowMap* pApp = new shadowMap();

    if (!pApp->Init()) {
        return 1;
    }

    pApp->Run();

    delete pApp;
 
    return 0;
}
