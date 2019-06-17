# Animator
Animator is a graphics class project that can render, model, and ray-trace objects in 3D space. Shaders can be applied to the objects so that they interact with light in different ways (see **Implemented Shaders** for more on the shaders). However, the shaders currently do not affect the ray-traced image. Primitive objects such as, cubes and spheres, were provided in the starter code, however the code to create a surface of revolution was implemented by my project partner and I. \
The animator elements of Animator use parametric curves to interpolate movement between keyframes. The curves include Bezier, Catmull-Rom, and B-Splines. Each type of curve features wrapping, in order to create smooth movements between replays.

**NB: A .exe of the project will be uploaded shortly.**

### Technologies
* C++
* OpenGL/GLSL

### Implemented Shaders
So far I have implemented 3 shaders for the modeler:
* Blinn-Phong Shader
* Cartoon Shader (with darkened silhouettes)
* Normal Mapping Shader
* Wobble Vertex Shader

The Blinn-Phong shader implements the Blinn-Phong illumination model with variable attenuation coefficients for point lighting. Variables also control the shininess of the object. \
The cartoon shader colors each pixel based on the intensity of light at that given pixel. There are 4 thresholds at >95%, 50-95%, 25-50% and <25%. The shader works for both direction and point lighting. Given its nature, objects using the cartoon shader do not have variable specularity and diffuse properties. \
The normal mapping shader uses a normal map to create the appearance of bumps by extracting a normal from the color of the normal map and converting all other vectors to tangest space before computing shading. \
The wobble vertex shader uses sine waves to give the impression that the object is wobbling without changing the mesh of the object.

### How to use
To change the sphere's shader, click the sphere, Material, and select the material of choice, such as Blinn-Phong, Toon, etc.
  
### Known Bugs
1. The starter code contains a bug where the uv-mapping for all primitive objects is off by a single band. I am in the process of fixing this bug.
2. When running animations, ray-tracer has been known to cover most of the image in red. A current work around is to avoid using ray-tracing animation frames.
