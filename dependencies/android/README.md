# ANDROID SETUP

* Clone the repo, then install the dependencies listed below
* Ensure the symbolic links in `[root]/android/app/jni` are correct. If not, fix them with the following on Windows:
  * `mklink /D src ..\..\..`
  * `mklink /D SDL ..\..\..\dependencies\android\SDL`
* Open `[root]/android/` in Android Studio, install the NDK and everything else that it asks for, and build.


## Dependencies:
SDL2: https://www.libsdl.org/download-2.0.php
Download the source code and unzip it in "./SDL/".

libogg: https://xiph.org/downloads/ (libogg)
Download libogg and unzip it in "./libogg/".

libtheora: https://xiph.org/downloads/ (libtheora)
Download libvorbis and unzip it in "./libtheora/".