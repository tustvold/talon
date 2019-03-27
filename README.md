# Talon

Highly experimental compile-time ECS engine using vulkan.

Written largely to experiment with vulkan and `boost::hana` and NOT SUITABLE FOR PRODUCTION USE.

## Shader Compilation

Currently the shader compilation is not hooked into CMake, it is therefore necessary to manually compile the shaders for the example program, located in `example/resources/shaders` using `glslangValidator`.