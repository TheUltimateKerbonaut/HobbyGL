# HobbyGL
A hobbyist OpenGL game engine written in C++

<img src="https://raw.githubusercontent.com/TheUltimateKerbonaut/HobbyGL/master/Screenshots/Screenshot1.png" alt="Screenshot of engine running a demo scene - it looks rather good!">

## Features
Amongst other things, the engine supports:
* A highly customisable deferred rendering pipeline
* Numerous point light and directional shadows with phong shading and attenuation
* Numerous point and directional lights using PCF soft shadows
* A comprehensive post-processing pipeline featuring bloom, chromatic abberation, depth of field, dithering, HDR and gamma correction.
* Cubemap reflections with the Fresnel effect
* Comprehensive model loading with Assimp
* In-game graphical debugging with ImGUI
* Normal mapping
* Specular mapping
* Resolution scaling
* A minimal executable size of under 650kB (in Release mode, as of the time of this README being written) 

## Usage
To compile, simply open the solution in Visual Studio, taking care to include dependencies, and build. Then run the executable, providing assets in the `res` folder, and the engine's (or your own) shaders in the `Shaders`.

## Supported platforms
* Developed and tested on Windows
* Due to the nature of OpenGL, porting to Linux or MacOS should be trivial, as all libraries used are cross-platform.

## Dependencies
* Assimp - for loading models
* GLFW3 - for creating windows and OpenGL
* ImGUI - for debugging
* glm - for maths operations
* Glad - for OpenGL
* StbImage (included in source code) - for importing images
Please see the dependencies' soure code for licenses, as well as their respecitve websites if appliable.