# How to use "ogldev_util.h" in Linux

> Actually, all head function are included in Include folder, but here "ogldev_util.h" has a source file "ogldev_util.cpp". So how can we overcome this problem in tutorial_04?

1. You muse download the lib library writing by auther of tutorials [OpenGL Step by Step](http://ogldev.atspace.co.uk/), and you can download the lib from web: http://ogldev.atspace.co.uk/ogldev-source.zip. In fact, we did it in the last section.


2. And then we copy the folder "Common" to the source directory:
> RootDir: OpenGLStepByStep
> SecondDir: Common/Include/tutorial_01/tutorial_02/.../tutorial_N


3. To use the folder "Include", we must include this folder in CMakeLists.txt:
> include_directories(${PROJECT_SOURCE_DIR}/Include)


4. To use "ogldev_util.h" in tutorial_*, you just need to include the head:
> #include "ogldev_util.h"

5. And then we failed to compile the target, since we have a bug:
> CMakeFiles/shadeTriangle.dir/shadeTriangle.cpp.o：在函数‘CompileShaders()’中：
shadeTriangle.cpp:(.text+0x209)：对‘ReadFile(char const*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&)’未定义的引用
shadeTriangle.cpp:(.text+0x222)：对‘ReadFile(char const*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&)’未定义的引用
collect2: error: ld returned 1 exit status
src/tutorial_04/CMakeFiles/shadeTriangle.dir/build.make:88: recipe for target '../bin/shadeTriangle' failed
make[2]: *** [../bin/shadeTriangle] Error 1
CMakeFiles/Makefile2:258: recipe for target 'src/tutorial_04/CMakeFiles/shadeTriangle.dir/all' failed

6. To fix this bug, we just include the source file to CMakeLists.txt:
>   ADD_EXECUTABLE(shadeTriangle 
    \${PROJECT_SOURCE_DIR}/shadeTriangle.cpp
    \${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp)

7. Ok, it compiles successfully.