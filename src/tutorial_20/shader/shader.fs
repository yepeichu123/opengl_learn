#version 330

const int MAX_POINT_LIGHTS = 2;                                                     

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

out vec4 FragColor;

struct BaseLight                                                                    
{                                                                                   
    vec3 Color;                                                                     
    float AmbientIntensity;                                                         
    float DiffuseIntensity;                                                         
};                                                                                  
                                                                                    
struct DirectionalLight                                                             
{                                                                                   
    BaseLight Base;                                                                 
    vec3 Direction;                                                                 
};                                                                                  
                                                                                    
struct Attenuation                                                                  
{                                                                                   
    float Constant;                                                                 
    float Linear;                                                                   
    float Exp;                                                                      
};                                                                                  
                                                                                    
struct PointLight                                                                           
{                                                                                           
    BaseLight Base;                                                                         
    vec3 Position;                                                                          
    Attenuation Atten;                                                                      
};

// 纹理采样器
uniform sampler2D gSampler;
// 平行光源参数
uniform DirectionalLight gDirectionalLight;
// 镜面反射参数
uniform vec3 gEyeWorldPos;                                                          
uniform float gMatSpecularIntensity;                                                
uniform float gSpecularPower;
// 点光源参数
uniform int gNumPointLights; 
uniform PointLight gPointLights[MAX_POINT_LIGHTS];                                          


vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal)                   
{                                               
    // 环境光颜色参数的计算没有变化,我们计算并存储它                                            
    vec4 AmbientColor = vec4(Light.Color * Light.AmbientIntensity, 1.0f);
    // 漫反射光强度
    float DiffuseFactor = dot(Normal, -LightDirection);                                     

    // 初始化漫反射光和镜面反射光参数                                                                         
    vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                                  
    vec4 SpecularColor = vec4(0, 0, 0, 0);                                                  
                                                                                            
    if (DiffuseFactor > 0) {         
        // 计算漫反射光                                                       
        DiffuseColor = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor, 1.0f);    

        // 计算镜面反射光                                                                     
        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);                             
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));                     
        float SpecularFactor = dot(VertexToEye, LightReflect);                                      
        if (SpecularFactor > 0) {                                                           
            SpecularFactor = pow(SpecularFactor, gSpecularPower);
            SpecularColor = vec4(Light.Color * gMatSpecularIntensity * SpecularFactor, 1.0f);
        }                                                                                   
    }                                                                                       

    // 返回平行光照、漫反射光和镜面反射光总和                                                   
    return (AmbientColor + DiffuseColor + SpecularColor);                                   
}                                                                                           

// 计算平行光照
vec4 CalcDirectionalLight(vec3 Normal)                                                      
{                                                                                           
    return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal); 
}                                                                                           

// 计算点光源光照
vec4 CalcPointLight(int Index, vec3 Normal)                                                 
{                                                               
    // 光照方向                            
    vec3 LightDirection = WorldPos0 - gPointLights[Index].Position;    
    // 渲染位置与点光源的距离                     
    float Distance = length(LightDirection);                                                
    LightDirection = normalize(LightDirection);                                             

    // 计算点光源反射光
    // 同时考虑光照衰减                                                                    
    vec4 Color = CalcLightInternal(gPointLights[Index].Base, LightDirection, Normal);       
    float Attenuation =  gPointLights[Index].Atten.Constant +                               
                         gPointLights[Index].Atten.Linear * Distance +                      
                         gPointLights[Index].Atten.Exp * Distance * Distance;               
                                                                                            
    return Color / Attenuation;                                                             
}                                                                                           
                                                                                            
void main()                                                                                 
{                                                                                           
    vec3 Normal = normalize(Normal0);                                                       
    vec4 TotalLight = CalcDirectionalLight(Normal);                                         
                                                                                            
    for (int i = 0 ; i < gNumPointLights ; i++) {                                           
        TotalLight += CalcPointLight(i, Normal);                                            
    }                                                                                       
                                                                                            
    FragColor = texture2D(gSampler, TexCoord0.xy) * TotalLight;                             
}