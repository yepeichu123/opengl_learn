# How to use SetCamera(...) function in Linux

> In tutorial_13, we use p.SetCamera(*pGameCamera), but pGameCamera = NULL. And we fall into segment fault.

1. Since we set values CameraPos and CameraTarget as well as CameraUp, but we don't pass these values to pGameCamera. 
Even the value pGameCamera is a empty pointer.

2. We search the override function "SetCamera", and we set:
> p.SetCamera(CameraPos, CameraTarget, CameraUp);

And then we work.