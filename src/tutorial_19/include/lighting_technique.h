#ifndef LIGHTING_TECHNIQUE_H
#define LIGHTING_TECHNIQUE_H

#include "technique.h"
#include "ogldev_math_3d.h"

struct DirectionalLight
{
    Vector3f Color;
    float AmbientIntensity;
    Vector3f Direction;
    float DiffuseIntensity;
};

class LightingTechnique : public Technique
{
public:

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

private:

    GLuint m_WVPLocation;
    GLuint m_samplerLocation;
    // 新添加世界坐标系的变换矩阵
    GLuint m_WorldMatrixLocation;
    // 镜面反射光相关参数
    GLuint m_eyeWorldPosLocation;
    GLuint m_matSpecularIntensityLocation;
    GLuint m_matSpecularPowerLocation;

    // 将平行光照和位置都封装进一个结构体中
    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint Direction;
        GLuint DiffuseIntensity;
    } m_dirLightLocation;
};

#endif // !LIGHTING_TECHNIQUE_H
