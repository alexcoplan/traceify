# traceify

traceify is a simple raytracer written in C++.

## Features

 - Geometric Primitives: Spheres, infinite planes
 - Shading: Diffuse (Lambertian), Specular (Phong)
 - Point Lights
 - Shadows
 - Reflections
 - Arbitary camera positioning and rotation
 - Simple bounding boxes for groups of primitives (clusters)
 - Super-sampling: 4x, adaptive up to 16x and 64x with optional jitter

## Short-term goals

 - Add more primitives: Triangles, Cylinders and Tori
 - Efficient bounding-box hierarchy
 - Transparency
 - Refraction
 - Soft shadows (area lights)
 - Depth of field
 - Texture Mapping
 - Bump mapping
 - Multi-threaded rendering

## Long-term goals

 - Add Bézier Patches as primitives
 - Separate scene data from code i.e. create an external scene data format
 - Triangle-based mesh representaiton
 - Displacement mapping
 - Ability to import meshes
 - Add the ability to render on the GPU 
 - Work towards being able to import scenes from Blender?
