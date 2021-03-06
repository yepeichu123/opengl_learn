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
3. AntTweakBar
4. ImageMagick-6
5. freetype
6. assimp
7. 



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
11. **tutorial_11**: drawing a transforming pyramid with three types trasnformation including scale and rotation as well as translation.
12. **tutorial_12**: drawing a perspective pyramid with rotation along y axis.
13. **tutorial_13**: drawing a perspective pyramid with fixed camera position and view.
14. **tutorial_14**: drawing a perspective pyramid which can be controled by keyboard.
15. **tutorial_15**: drawing a perspective pyramid which can be controled by mouse.
16. **tutorial_16**: drawing a perspective pyramid with texture.
17. **tutorial_17**: drawing a perspective pyramid with texture and directional light.
18. **tutorial_18**: drawing a perspective pyramid with texture and directional light as well as diffuse light.
19. **tutorial_19**: drawing a perspective pyramid with directional light and diffuse light as well as specular light.
20. **tutorial_20**: drawing a texture image with two point lights.
21. **tutorial_21**: drawing a texture image with two point lights and two spot lights.
22. **tutorial_22**: Loading a 3D model using Assimp Library.
23. **tutorial_23**: constructing a shadow map from a 3d model.
24. **tutorial_24**: constructing a shadow map from a 3d model and display with 3d model.
25. **tutorial_25**: drawing a sky box.
26. 



### List of docs

1. **fix_math_3d_bug.md**.
2. **fix_util_bug.md**.
3. **fix_pipeline_bug.md**.
4. **fix_cameraModel_bug.md**.
5. **fix_newCameraObject_bug.md**.
6. **fix_imageMagick_bug.md**.
7. **fix_assimp_bug.md**.
8. 



### Note 

Pay attention to the shader file path, please change the path to your own:
> 
> const char* pVSFileName = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_**x**/shader/shader.vs";
> 
> const char* pFSFileName = "/home/xiaoc/xiaoc/code/opengl/stepByStep/src/tutorial_**x**/shader/shader.fs";
> 