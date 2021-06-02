#include "lighting_technique.h"

LightingTechnique::LightingTechnique()
{   

}

bool LightingTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "lighting.vs")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "lighting.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    m_WVPLocation = GetUniformLocation("gWVP");
    m_samplerLocation = GetUniformLocation("gSampler");
    // 修改平行光照强度和位置,将其融入结构体中
    // 获取光照相关的变量输入
    m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Color");
    m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.AmbientIntensity");
    m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
    m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.DiffuseIntensity");
    // 镜面反射相关参数
    m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
    m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
    m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");

    // 验证数据有效性
    if (m_dirLightLocation.Color == 0xFFFFFFFF ||
        m_dirLightLocation.AmbientIntensity == 0xFFFFFFFF ||
        m_dirLightLocation.Direction == 0xFFFFFFFF ||
        m_dirLightLocation.DiffuseIntensity == 0xFFFFFFFF ||
        m_WVPLocation == 0xFFFFFFFF ||
        m_samplerLocation == 0xFFFFFFFF ||
        m_eyeWorldPosLocation == 0xFFFFFFFF ||
        m_matSpecularIntensityLocation == 0xFFFFFFFF ||
        m_matSpecularPowerLocation == 0xFFFFFFFF)
    {
        return false;
    }

    return true;
}

bool LightingTechnique::Init(const char* pvertexFile, const char* pfragFile)
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, pvertexFile)) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, pfragFile)) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    m_WVPLocation = GetUniformLocation("gWVP");
    m_WorldMatrixLocation = GetUniformLocation("gWorld");
    m_samplerLocation = GetUniformLocation("gSampler");
    // 修改平行光照强度和位置,将其融入结构体中
    // 获取光照相关的变量输入
    m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Color");
    m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.AmbientIntensity");
    m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
    m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.DiffuseIntensity");
    // 镜面反射相关参数
    m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
    m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
    m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");

    // 验证数据有效性
    if (m_dirLightLocation.Color == 0xFFFFFFFF ||
        m_dirLightLocation.AmbientIntensity == 0xFFFFFFFF ||
        m_dirLightLocation.Direction == 0xFFFFFFFF ||
        m_dirLightLocation.DiffuseIntensity == 0xFFFFFFFF ||
        m_WVPLocation == 0xFFFFFFFF ||
        m_samplerLocation == 0xFFFFFFFF ||
        m_eyeWorldPosLocation == 0xFFFFFFFF ||
        m_matSpecularIntensityLocation == 0xFFFFFFFF ||
        m_matSpecularPowerLocation == 0xFFFFFFFF)
    {
        return false;
    }

    return true;
}

// 设置模型-视图矩阵
void LightingTechnique::SetWVP(const Matrix4f& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);    
}

// 设置世界坐标系变换矩阵
void LightingTechnique::SetWorldMatrix(const Matrix4f& WorldInverse)
{
    glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)WorldInverse.m);
}

// 设置纹理单元
void LightingTechnique::SetTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_samplerLocation, TextureUnit);
}

// 设置光照
void LightingTechnique::SetDirectionalLight(const DirectionalLight& Light)
{
    // 给uniform变量赋值
    glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
    // 法向量归一化
    Vector3f Direction = Light.Direction;
    Direction.Normalize();
    glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
    glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}

void LightingTechnique::SetEyeWorldPos(const Vector3f& EyeWorldPos)
{
    glUniform3f(m_eyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}

void LightingTechnique::SetMatSpecularIntensity(float Intensity)
{
    glUniform1f(m_matSpecularIntensityLocation, Intensity);
}

void LightingTechnique::SetMatSpecularPower(float Power)
{
    glUniform1f(m_matSpecularPowerLocation, Power);
}