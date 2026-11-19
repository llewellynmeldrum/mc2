# mc vulkan rewrite (mc2/vmc)
rewriting my minecraft style voxel renderer from scratch with a focus on distant horizons level scale (LODs), maximum performance with Vulkan, and better abstractions from the start.

Goals this time:
- 256 to 512 render distance running on my M1 Macbook Pro @60fps 

Techniques i will use to achieve this:
- Generated LODs which average the color of a texture over however many faces it is built to cover
- Frequent, detailed benchmarks and profiling (Tracy for retrospective profiling + my own realtime in-engine graphs)
- vertex pulling or 1:1 vertex:generated face ratio
- Heavily compressed vertex data





# Building
Non-vendored requirements are: 
- cmake and ninja 
- A C++26-capable compiler, e.g gcc16+
- Vulkan 1.4+ SDK
- SDL3 




## Resources
// todo: add links later
vulkan spec
vulkan spec tutorial
vk-guide tutorial

- [X] chapter 1 ([8b6a13](https://github.com/llewellynmeldrum/vkguide-follow-along/commit/c01598bb0b15dc9d3b2fbc5d85b50c3fc18b6a13))
- [X] chapter 2 ([92a4d0](https://github.com/llewellynmeldrum/vkguide-follow-along/commit/ddb8b6b257b15fce535af95938cc9d54c492a4d0)) 

Switched over to the vulkan spec tutorial [docs.vulkan.org/tutorial](https://docs.vulkan.org/tutorial/latest/)

