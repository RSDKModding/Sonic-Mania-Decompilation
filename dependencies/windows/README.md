# WINDOWS SETUP

* Clone the repo, then install the dependencies listed below.
* Build via the Visual Studio solution (or grab a prebuilt executable from the releases section.)

## Dependencies:

* libogg: https://xiph.org/downloads/ (libogg)

    Download libogg and unzip it in "./libogg/", then build the static library

* libtheora: https://xiph.org/downloads/ (libtheora)

    Download libtheora and unzip it in "./libtheora/", then build the VS2010 static library (win32/VS2010/libtheora_static.sln)

* SDL2 (Required for SDL2 backends): https://www.libsdl.org/download-2.0.php

    Download the appropriate development library for your compiler and unzip it in "./SDL2/"

* For the GL3 backends, visit the GL3 readme [here.](../gl3/README.md)