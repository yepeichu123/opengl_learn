#ifndef LIGHTING_TECHNIQUE_H
#define LIGHTING_TECHNIQUE_H

#include "technique.h"
#include "ogldev_math_3d.h"

struct BaseLight
{
    Vector3f Color;
    float AmbientIntensity;
    float DiffuseIntensity;

    BaseLight()
    {
        Color = Vector3f(0.0f, 0.0f, 0.0f);
        AmbientIntensity = 0.0f;
        DiffuseIntensity = 0.0f;
    }
};

struct DirectionalLight: public BaseLight
{
    Vector3f Direction;
    DirectionalLight()
    {
        Direction = Vector3f(0.0f, 0.0f, 0.0f);
    }
};

struct PointLight : public BaseLight
{
    Vector3f Position;

    struct
    {
        float Constant;
        float Linear;
        float Exp;
    } Attenuation;

    PointLight()
    {
        Position = Vector3f(0.0f, 0.0f, 0.0f);
        Attenuation.Constant = 1.0f;
        Attenuation.Linear = 0.0f;
        Attenuation.Exp = 0.0f;
    }
};

class LightingTechnique : public Technique
{
public:

    static const unsigned int MAX_POINT_LIGHTS = 2;

    LightingTechnique();

    virtual bool Init();
    virtual bool Init(const char* pvertexFile, const char* pfragFile);

    // 环境光相关
    void SetWVP(const Matrix4f& WVP);
    void SetTextureUnit(unsigned int TextureUnit);
    // 漫反射相关
    void SetWorldMatrix(const Matrix4f& WVP);
    void SetDirectionalLight(const DirectionalLight& Light);
    // 镜面反射相关
    void SetEyeWorldPos(const Vector3f& EyeWorldPos);
    void SetMatSpecularIntensity(float Intensity);
    void SetMatSpecularPower(float Power);
    // 设置点光源
    // 输入参数包括点光源的数量和点光源结构体
    void SetPointLights(unsigned int NumLights, const PointLight* pLights);

private:

    GLuint m_WVPLocation;
    GLuint m_samplerLocation;
    // 新添加世界坐标系的变换矩阵
    GLuint m_WorldMatrixLocation;
    // 镜面反射光相关参数
    GLuint m_eyeWorldPosLocation;
    GLuint m_matSpecularIntensityLocation;
    GLuint m_matSpecularPowerLocation;
    // 点光源
    GLuint m_numPointLightsLocation;

    // 将平行光照和位置都封装进一个结构体中
    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint Direction;
        GLuint DiffuseIntensity;
    } m_dirLightLocation;

    // 点光源结构
    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Position;
        struct
        {
            GLuint Constant;
            GLuint Linear;
            GLuint Exp;
        } Atten;
    } m_pointLightsLocation[MAX_POINT_LIGHTS];
};

#endif // !LIGHTING_TECHNIQUE_H
