# 3D Modeler
3D Modeler is a graphics class project that can render and model objects in 3D space. Shaders can be applied to the objects so that they interact with light in different ways (see **Implemented Shaders** for more on the shaders). Primitive objects such as, cubes and spheres, were provided in the starter code, however the code to create a surface of revolution was implemented by my project partner and I.

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
To change the sphere's shader, click \<I WILL NEED TO CHECK\> and select the material of choice, such as Blinn-Phong, Toon, etc.
  
### Known Bugs
The starter code contains a bug where the uv-mapping for all primitive objects is off by a single band. I am in the process of fixing this bug.
