* Clone the repo, then install the dependencies listed below, then build via the visual studio solution (or grab a prebuilt executable from the releases section.)

## Dependencies:

* libogg: https://xiph.org/downloads/ (libogg)
download libogg and unzip it in "./libogg/", then build the static library

* SDL2 (Required for SDL2 backends): https://www.libsdl.org/download-2.0.php
download the appropriate development library for your compiler and unzip it in "./SDL2/"

* GLAD (Required for OpenGL backends):
Go to https://glad.dav1d.de/ and generate GL version 3.3 for C/C++. Download glad.zip and extract it's contents to ./glad/

* GLFW (Required for OpenGL backends):
Go to https://www.glfw.org/download.html and download the 64-bit or 32-bit binaries. Make sure what you're building matches the binaries use download!
You're also free to build it yourself.