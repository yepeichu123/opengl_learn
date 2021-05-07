# How to create Camera object in Linux?

> In tutorial_13, we use p.SetCamera(*pGameCamera), but pGameCamera = NULL. And we fall into segment fault. Finally, we use another constructor to fix this bug.
>

> p.SetCamera(CameraPos, CameraTarget, CameraUp);

-------------------------------------------------------------------

However, we can use another method to allocate memory to the empty pointer "pGameCamera" like this:

>

1. pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT); (in main function)

2. p.SetCamera(*pGameCamera). (in RenderSceneCB)


&ensp; &ensp;But we fall into another problem:

> CMakeFiles/cameraCoordinate.dir/cameraCoordinate.cpp.o：在函数‘main’中：
cameraCoordinate.cpp:(.text.startup+0x85)：对‘Camera::Camera(int, int)’未定义的引用
collect2: error: ld returned 1 exit status
src/tutorial_13/CMakeFiles/cameraCoordinate.dir/build.make:133: recipe for target '../bin/cameraCoordinate' failed
make[2]: *** [../bin/cameraCoordinate] Error 1
CMakeFiles/Makefile2:690: recipe for target 'src/tutorial_13/CMakeFiles/cameraCoordinate.dir/all' failed
make[1]: *** [src/tutorial_13/CMakeFiles/cameraCoordinate.dir/all] Error 2
Makefile:86: recipe for target 'all' failed
make: *** [all] Error 2

>

We search a lot files to find the problem and finally find the "AntTweakBar.h" is needed. Compiling the source files together as usual is our beginning method. However, there is no source file about AntTweakBar. 

So we have to link the library libAntTweakBar.so in root_dir/Lib. 

1. We should include the Lib into our porject. First of all, we add the common in the CMakeLists.txt of root directory:

> link_directories(${PROJECT_SOURCE_DIR}/Lib)

2. And then we should link the "AntTweakBar" to our tutorial_14 project:

> target_link_libraries(
    cameraCoordinate 
    ${OPENGL_LIBRARY}
    -lglut
    -lGLEW
    AntTweakBar)

3. Compiling this project but failed again because we fall into another problem:

> CMakeFiles/cameraCoordinate.dir/cameraCoordinate.cpp.o：在函数‘main’中：
cameraCoordinate.cpp:(.text.startup+0x85)：对‘Camera::Camera(int, int)’未定义的引用
collect2: error: ld returned 1 exit status
src/tutorial_13/CMakeFiles/cameraCoordinate.dir/build.make:133: recipe for target '../bin/cameraCoordinate' failed
make[2]: *** [../bin/cameraCoordinate] Error 1
CMakeFiles/Makefile2:690: recipe for target 'src/tutorial_13/CMakeFiles/cameraCoordinate.dir/all' failed
make[1]: *** [src/tutorial_13/CMakeFiles/cameraCoordinate.dir/all] Error 2
Makefile:86: recipe for target 'all' failed
make: *** [all] Error 2


4. Ok, this bug is the same as "fix_pipeline_bug.md", we utilize the same method to fix it:

> add_executable(cameraCoordinate 
    \${PROJECT_SOURCE_DIR}/cameraCoordinate.cpp
    \${PROJECT_SOURCE_DIR}/../../Common/ogldev_util.cpp
    \${PROJECT_SOURCE_DIR}/../../Common/math_3d.cpp
    \${PROJECT_SOURCE_DIR}/../../Common/pipeline.cpp
    \${PROJECT_SOURCE_DIR}/../../Common/camera.cpp
    \${PROJECT_SOURCE_DIR}/../../Common/ogldev_atb.cpp)

>
And then we compile again and it works.

> ...
[ 95%] Building CXX object src/tutorial_13/CMakeFiles/cameraCoordinate.dir/__/__/Common/camera.cpp.o
[ 97%] Building CXX object src/tutorial_13/CMakeFiles/cameraCoordinate.dir/__/__/Common/ogldev_atb.cpp.o
[100%] Linking CXX executable ../../../bin/cameraCoordinate
[100%] Built target cameraCoordinate
