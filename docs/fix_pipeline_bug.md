# How to use "ogldev_pipeline.h" in Linux

> From the lecture: [一步步学OpenGL(11) -《复合变换》](https://zhuanlan.zhihu.com/p/148981182), we had some problems occured when we compiled. The article informs that we should include the head function "ogldev_pipeline.h" in our implemented function and link the library "AntTweakBar". However, we tried many ways to link the library but failed. So how can we overcome this problem in tutorial_11? 

1. You muse download the library writing by auther of tutorials [OpenGL Step by Step](http://ogldev.atspace.co.uk/), and you can download the lib from web: http://ogldev.atspace.co.uk/ogldev-source.zip. In fact, we did it at the beginning time.


2. The Common folder is in the root directory. 
   And then we copy the folder "Common" to the source directory:
> RootDir: OpenGLStepByStep
> SecondDir: Common/Include/tutorial_01/tutorial_02/.../tutorial_N


3. To use the folder "Include", we must include this folder in CMakeLists.txt:
> include_directories(${PROJECT_SOURCE_DIR}/Include)


4. To use "ogldev_pipeline.h" in tutorial_*, you just need to include the head:
> #include "ogldev_pipeline.h"

5. And then we failed to compile the target, since we have a bug:
> CMakeFiles/transformPyramid.dir/transformPyramid.cpp.o：在函数‘RenderSceneCB()’中：
transformPyramid.cpp:(.text+0xb0)：对‘Pipeline::GetWorldTrans()’未定义的引用
collect2: error: ld returned 1 exit status
src/tutorial_11/CMakeFiles/transformPyramid.dir/build.make:103: recipe for target '../bin/transformPyramid' failed
make[2]: *** [../bin/transformPyramid] Error 1
CMakeFiles/Makefile2:594: recipe for target 'src/tutorial_11/CMakeFiles/transformPyramid.dir/all' failed
make[1]: *** [src/tutorial_11/CMakeFiles/transformPyramid.dir/all] Error 2
Makefile:86: recipe for target 'all' failed
make: *** [all] Error 2


6. To fix this bug, we just include the source file to CMakeLists.txt instead linking the library "AntTweakBar". You just need to write the code like this:
>   ADD_EXECUTABLE(shadeTriangle 
    \${PROJECT_SOURCE_DIR}/shadeTriangle.cpp
    \${PROJECT_SOURCE_DIR}/../../Common/ogldev_pipeline.cpp)

7. Oh my god, we get more bugs than ever before:
> CMakeFiles/transformPyramid.dir/__/__/Common/pipeline.cpp.o：在函数‘Pipeline::GetProjTrans()’中：
pipeline.cpp:(.text+0x10)：对‘Matrix4f::InitPersProjTransform(PersProjInfo const&)’未定义的引用
CMakeFiles/transformPyramid.dir/__/__/Common/pipeline.cpp.o：在函数‘Pipeline::GetWorldTrans()’中：
pipeline.cpp:(.text+0x50)：对‘Matrix4f::InitScaleTransform(float, float, float)’未定义的引用
pipeline.cpp:(.text+0x69)：对‘Matrix4f::InitRotateTransform(float, float, float)’未定义的引用
pipeline.cpp:(.text+0x85)：对‘Matrix4f::InitTranslationTransform(float, float, float)’未定义的引用
CMakeFiles/transformPyramid.dir/__/__/Common/pipeline.cpp.o：在函数‘Pipeline::GetViewTrans()’中：
pipeline.cpp:(.text+0x472)：对‘Matrix4f::InitTranslationTransform(float, float, float)’未定义的引用
pipeline.cpp:(.text+0x484)：对‘Matrix4f::InitCameraTransform(Vector3f const&, Vector3f const&)’未定义的引用
CMakeFiles/transformPyramid.dir/__/__/Common/pipeline.cpp.o：在函数‘Pipeline::GetVPTrans()’中：
pipeline.cpp:(.text+0x6d9)：对‘Matrix4f::InitPersProjTransform(PersProjInfo const&)’未定义的引用
CMakeFiles/transformPyramid.dir/__/__/Common/pipeline.cpp.o：在函数‘Pipeline::GetWVPTrans()’中：
pipeline.cpp:(.text+0x947)：对‘Matrix4f::InitPersProjTransform(PersProjInfo const&)’未定义的引用
CMakeFiles/transformPyramid.dir/__/__/Common/pipeline.cpp.o：在函数‘Pipeline::GetWVOrthoPTrans()’中：
pipeline.cpp:(.text+0xdb3)：对‘Matrix4f::InitOrthoProjTransform(OrthoProjInfo const&)’未定义的引用
CMakeFiles/transformPyramid.dir/__/__/Common/pipeline.cpp.o：在函数‘Pipeline::GetWPTrans()’中：
pipeline.cpp:(.text+0x13eb)：对‘Matrix4f::InitPersProjTransform(PersProjInfo const&)’未定义的引用
collect2: error: ld returned 1 exit status
src/tutorial_11/CMakeFiles/transformPyramid.dir/build.make:118: recipe for target '../bin/transformPyramid' failed
make[2]: *** [../bin/transformPyramid] Error 1
CMakeFiles/Makefile2:594: recipe for target 'src/tutorial_11/CMakeFiles/transformPyramid.dir/all' failed
make[1]: *** [src/tutorial_11/CMakeFiles/transformPyramid.dir/all] Error 2
Makefile:86: recipe for target 'all' failed
make: *** [all] Error 2

8. To fix these bugs, we find that "ogldev_pipeline.cpp" depends on the source file ""math_3d.cpp". Therefore, we tried to include the source file "math_3d.cpp" in our project and compiled. Finally, we successfully overcome this problem:
>  ADD_EXECUTABLE(shadeTriangle 
    \${PROJECT_SOURCE_DIR}/shadeTriangle.cpp
    \${PROJECT_SOURCE_DIR}/../../Common/ogldev_pipeline.cpp
    \${PROJECT_SOURCE_DIR}/../../Common/math_3d.cpp
    \${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp)

> ...
[ 93%] Built target indexDrawing
[ 96%] Building CXX object src/tutorial_11/CMakeFiles/transformPyramid.dir/__/__/Common/pipeline.cpp.o
[100%] Linking CXX executable ../../../bin/transformPyramid
[100%] Built target transformPyramid
