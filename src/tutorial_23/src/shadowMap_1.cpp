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
#include "shadow_map_technique.h"
#include "mesh.h"
#include "shadow_map_fbo.h"

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1200

using namespace std;

const char* pVSFileName     = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_23/shader/shader.vs";
const char* pFSFileName     = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_23/shader/shader.fs";
const char* pTextureModel   = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_23/model/quad.obj";
const char* pTextureFile    = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_23/model/phoenix_ugv.md2";


class shadowMap : public ICallbacks//, public OgldevApp
{
public:
    // 初始化各项参数,包括光照等参数,观测窗口等
    shadowMap()
    {
        m_pShadowMapTech = NULL;
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
        SAFE_DELETE(m_pShadowMapTech);
        SAFE_DELETE(m_pGameCamera);
        SAFE_DELETE(m_pMesh);
        SAFE_DELETE(m_pQuad);
    }

    bool Init()
    {
        if (!m_shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
            return false;
        }

        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT/*, Pos, Target, Up*/);
     
        m_pShadowMapTech = new ShadowMapTechnique();

        // 创建着色器并关联uniform变量
        if (!m_pShadowMapTech->Init(pVSFileName, pFSFileName)) {
            printf("Error initializing the shadow map technique\n");
            return false;
        }        
        
        m_pShadowMapTech->Enable();
       
       
        m_pQuad = new Mesh();
		if (!m_pQuad->LoadMesh(pTextureModel)) {
            return false;
        }

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
        
        glutSwapBuffers();
    }

    // 阴影贴图渲染
    // 将深度信息渲染到shadow map纹理中
    virtual void ShadowMapPass()
    {
        // 绑定FBO，将所有深度值渲染到shadow map中
        m_shadowMapFBO.BindForWriting();

        glClear(GL_DEPTH_BUFFER_BIT);

        Pipeline p;
        p.Scale(0.1f, 0.1f, 0.1f);
        p.Rotate(0.0f, m_scale, 0.0f);
        p.WorldPos(0.0f, 0.0f, 5.0f);
        // 相机位置设置与光源在一位置
        p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
        p.SetPerspectiveProj(m_persProjInfo);
        m_pShadowMapTech->SetWVP(p.GetWVPTrans());
        m_pMesh->Render();
        
        // 切换回默认的frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    // 将阴影贴图显示到模型上
    virtual void RenderPass()
    {
        // 清除颜色和深度缓冲区
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
        // 使用纹理单元0，并绑定阴影贴图用来读取其中的数据
        m_pShadowMapTech->SetTextureUnit(0);
        m_shadowMapFBO.BindForReading(GL_TEXTURE0);

        Pipeline p;
        p.Scale(5.0f, 5.0f, 5.0f);
        p.WorldPos(0.0f, 0.0f, 10.0f);
        // 相机自身位置
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
        p.SetPerspectiveProj(m_persProjInfo);
        m_pShadowMapTech->SetWVP(p.GetWVPTrans());

        m_pQuad->Render();       
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

    Camera* m_pGameCamera;
    float m_scale;
    ShadowMapTechnique* m_pShadowMapTech;
    SpotLight m_spotLight;
    Mesh* m_pMesh;
    Mesh* m_pQuad;
    ShadowMapFBO m_shadowMapFBO;
    PersProjInfo m_persProjInfo;
};


int main(int argc, char** argv)
{
    // 第三个参数必须设置为true,表示开启深度测试，否则不会出现阴影
    GLUTBackendInit(argc, argv, true, false);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "shadowMap_1")) {
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
