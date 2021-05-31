# How to link 3rdparty_ImageMagick-6 in Linux


> In tutorial_16, we use ImageMagick-6 to load image. However, the Include folder only contains head files without any source files.
> What's more, the author provides that .dll files which cannot be linked in linux. 


1. We should download and compile the source code:
>
>  [linux下ImageMagick安装和使用](https://blog.csdn.net/jiangxinyu/article/details/1698997?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_title-1&spm=1001.2101.3001.4242)
> 

2. After compiling the source code, we have installed it in our system. So we can link it in CMakeLists.txt:
> 
> target_link_libraries(
>    texture 
>    ...
>    Magick++-6.Q16
>)


3. And then we successfully compile and run our code.