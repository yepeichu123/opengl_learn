# How to use assimp library in Linux


> In tutorial_22, we use assimp library to load 3d models, but there is no library or source code in the code folder provided by author.
> Therefore, we should download and compile by ourself.


1. Go to the [assimp github](https://github.com/assimp/assimp).

2. Choosing the release version what you like and download:
> git clone https://github.com/assimp/assimp

3. Following the INSTALL file, we compile the project:
> cmake CMakeLists.txt -G 'Unix Makefiles'
> make -j8
> sudo make install

&ensp; &ensp;We install the library into our system, and then we can link the libassimp.so easilly.

4. To compile our project, we should include the assimp directory:

> include_directories(
>    \${PROJECT_SOURCE_DIR}/include
>    \${PROJECT_SOURCE_DIR}/../../Include/assimp
>    \${PROJECT_SOURCE_DIR}/../../Include/assimp/Compiler)

&ensp; &ensp;And linking our project:

> target_link_libraries(
>    shadowMap_1 
>    ..
>    assimp)

5. **Note:** Be carefully we should open GL_DEPTH_TEST, which is modified in our main function:

> GLUTBackendInit(argc, argv, **true**, false);


And then it works.
