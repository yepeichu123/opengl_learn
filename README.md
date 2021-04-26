# opengl_learn
##### ——————————————Peichu Ye / xiaoC : peichu.ye@gmail.com

### Learn OpenGL step by Step 

> This program is constructed to writing code following the course:
> 
> English: [OpenGL Step by Step - OpenGL Development](http://ogldev.atspace.co.uk/)
> 
> Chinese: [一步步学OpenGL](https://zhuanlan.zhihu.com/p/26230474)



### Dependencies
1. OpenGL
2. GLUT



### Project folder instruction

1. **bin**: executable files.
2. **Common**: Self-defined functions of course author.
3. **Include**: Self-defined functions of course author.
4. **docs**: Some fixed bugs when we compile the program.
5. **src**: Project of each tutorial.




### Build && run

You just need to run the command:
> ./make_project.bash

And then you can run the executable files in the root dir:
> ./bin/helloDot



### Course List

1. **tutorial_01**: drawing a window.
2. **tutorial_02**: drawing a point.
3. **tutorial_03**: drawing a triangular.
4. **tutorial_04**: drawing a triangular with red color.
5. **tutorial_05**: drawing a red triangular with variable in size by using uniform type.
6. **tutorial_06**: drawing a moving triangular with red color.
7. **tutorial_07**: drawing a rotating triangular with red color.
8. **tutorial_08**: drawing a scaling triangular with red color.
9. **tutorial_09**: drawing a scaling triangular with changing color by interpolation.
10. **tutorial_10**: drawing a rotating pyramid along y axis. 
11. **tutorial_11**: 



### List of docs

1. **fix_math_3d_bug.md**.
2. **fix_util_bug.md**.
3. 



### Note 

Pay attention to the shader file path, please change the path to your own:
> 
> const char* pVSFileName = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_06/shader/shader.vs";
> 
> const char* pFSFileName = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_06/shader/shader.fs";
> 