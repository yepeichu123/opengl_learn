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

#ifndef GLUT_BACKEND_H
#define	GLUT_BACKEND_H

#include "ogldev_types.h"
#include "ogldev_callbacks.h"

// GLUT 初始化
void GLUTBackendInit(int argc, char** argv, bool WithDepth, bool WithStencil);

// GLUT 创建指定参数的窗口
bool GLUTBackendCreateWindow(uint Width, uint Height, bool isFullScreen, const char* pTitle);

// 调用指定回调函数的主循环函数
void GLUTBackendRun(ICallbacks* pCallbacks);

void GLUTBackendSwapBuffers();

void GLUTBackendLeaveMainLoop();

OGLDEV_KEY GLUTKeyToOGLDEVKey(uint Key);

#endif	/* GLUT_BACKEND_H */

