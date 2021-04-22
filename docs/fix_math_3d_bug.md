# How to use math_3d in Linux

> To use "ogldev_math_3d.h" in tutorial_02 is a small trouble for novice of opengl. Therefore, we produce a method to those guys who get into trouble of this function.

1. You muse download the lib that auther of tutorials [OpenGL Step by Step](http://ogldev.atspace.co.uk/), and you can download the lib from web: http://ogldev.atspace.co.uk/ogldev-source.zip .


2. And then we copy the folder "Include" to the source directory:
> RootDir: OpenGLStepByStep
> SecondDir: Include/tutorial_01/tutorial_02/.../tutorial_N


3. To use the folder "Include", we must include this folder in CMakeLists.txt:
> include_directories(${PROJECT_SOURCE_DIR}/Include)


4. To use "ogldev_math_3d.h" in tutorial_*, you just need to include the head:
> #include "ogldev_math_3d.h"
