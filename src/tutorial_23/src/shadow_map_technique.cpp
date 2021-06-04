/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ogldev_util.h"
#include "shadow_map_technique.h"


ShadowMapTechnique::ShadowMapTechnique()
{
}

bool ShadowMapTechnique::Init(const char* pVSFileName, const char* pFSFileName)
{
    // 判断创建OpenGL程序是否成功
    if (!Technique::Init()) {
        return false;
    }

    // 读取并加载着色器
    if (!AddShader(GL_VERTEX_SHADER, pVSFileName)) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, pFSFileName)) {
        return false;
    }

    // 编译并链接着色器
    if (!Finalize()) {
        return false;
    }

    // 获得uniform变量
    m_WVPLocation = GetUniformLocation("gWVP");
    m_textureLocation = GetUniformLocation("gShadowMap");

    // 检查变量信息
    if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
        m_textureLocation == INVALID_UNIFORM_LOCATION) {
        return false;
    }

    return true;
}

// 设置坐标变换矩阵
void ShadowMapTechnique::SetWVP(const Matrix4f& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);
}

// 设置采用的纹理单元
void ShadowMapTechnique::SetTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_textureLocation, TextureUnit);
}
