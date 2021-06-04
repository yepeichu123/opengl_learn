#version 330                                                                        
                                                                                    
in vec2 TexCoordOut;                                                                
uniform sampler2D gShadowMap;                                                       
                                                                 
out vec4 FragColor;                                                                 
                                                                 
void main()                                                                         
{                                     
    // 采集固定位置的深度值                                              
    float Depth = texture(gShadowMap, TexCoordOut).x;      
    // 放大深度值                         
    Depth = 1.0 - (1.0 - Depth) * 25.0;    
    FragColor = vec4(Depth);                                                        
}
