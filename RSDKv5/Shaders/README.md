# RSDKv5 Shaders

These shaders have been decompiled back from their CSO objects into their HLSL equivalents. Both pre-plus and post-plus (which added screen dimming) have been decompiled.

## Compiling from HLSL

In order to compile the HLSL shaders, you must: 
* Add the DirectX HLSL compiler to PATH, of which is included in the Windows SDK from Windows 8 and onwards. It is located in `C:\Program Files (x86)\Windows Kits\10\bin\[SDK ver]\x86\fxc.exe` by default.
* Run the `compileVSO` and `compileFSO` batch files with the shader you want to compile as the first argument.

## Notice
Please note that the **only** official shaders are of DX9 and DX11. All other directories are ported from HLSL as to match the added render devices.

If you want to use the shaders of different render devices other than the Data.rsdk you own (ex. the OpenGL3 backend must have them to work,) it is recommended to make a mod with the files in `Data/Shaders/[folder name]`. The decompilation may compile and use them for you depending on if the render device supports it.