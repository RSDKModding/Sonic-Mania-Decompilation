# RSDKv5 Shaders

These shaders have been decompiled back from compiled bytecode objects into their HLSL equivalents. Both pre-plus (RSDKv5 Rev01) and post-plus (RSDKv5 Rev02, which added screen dimming) have been decompiled.

## Compiling from HLSL
* The decompilation will automatically compile loaded shader source files that are in `Data/Shaders/[folder name]/` during runtime.
* the DirectX shader compiler can be used to compile HLSL shader source code into compiled bytecode objects. Information on the DirectX shader compiler can be found [here](https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-part1).
* An example command for the DirectX shader compiler would be:
`fxc.exe /D RETRO_REV02=1 /T ps_3_0 /Fe log.txt /Zi /E PSMain /Fo example.fso example.hlsl`. This compiles the file `example.hlsl` as an HLSL model 3.0 pixel shader with an entry point of `PSMain` with post-plus (RSDKv5 Rev02) features enabled. More information on how to use the DirectX shader compiler from the command line can be found [here](https://docs.microsoft.com/en-us/windows/win32/direct3dtools/dx-graphics-tools-fxc-syntax).

## Notice
Please note that the **only** official shaders are of DX9 and DX11. All other directories are ported from HLSL as to match the added render devices.

If you want to use the shaders of different render devices other than the Data.rsdk you own (ex. the OpenGL3 backend must have them to work,) it is recommended to make a mod with the files in `Data/Shaders/[folder name]`. The decompilation may compile and use them for you depending on if the render device supports it.
