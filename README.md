# **SUPPORT THE OFFICIAL RELEASE OF SONIC MANIA (PLUS)**
+ Without assets from the official releases, this decompilation will not run.

+ You can get the official release of Sonic Mania (Plus) from:
  * Windows
    * [Via Steam](https://store.steampowered.com/app/584400/Sonic_Mania/)
    * [Via The Epic Games Store](https://www.epicgames.com/store/en-US/p/sonic-mania)
    * [Via Origin](https://www.origin.com/aus/en-us/store/sonic-the-hedgehog/sonic-mania)
  * [Switch, via the eShop](https://www.nintendo.com/games/detail/sonic-mania-switch/)
  * [PS4, via the Store](https://store.playstation.com/en-us/product/UP0177-CUSA07023_00-SONICMANIA000000)
  * [Xbox One, via the Store](https://www.xbox.com/en-US/games/store/sonic-mania/BXH46NQT9W4Q/0001)

Even if your platform isn't supported by the official releases, you **must** buy or officially download it for the assets.

## **DO NOT USE THIS DECOMPILATION PROJECT AS A MEANS TO PIRATE SONIC MANIA**
We do not condone using this project as a means for piracy in any form. This project was made with love and care for the source material and was created for purely educational purposes, and would not exist without the work of Sega, Headcannon, and Evening Star.

If you want to transfer your save from the official PC versions, you can just copy your savedata into the folder containing the decompilation!

# Additional Tweaks
* Added a built-in mod loader and API calls to enable a much smoother modding experience.
* Added support for targeting RSDKv5U rather than standalone RSDKv5
* Added all content from all released versions of the game. Including: 1.00 (Console initial release), 1.03 (PC initial release) & 1.06 (Plus update)

# How to Build
This repo includes everything you need for Sonic Mania + RSDKv5(U). It is recommended to clone this repo recursively using `git clone https://github.com/Rubberduckycooly/Sonic-Mania-Decompilation --recursive` to build both immediately.

If you wish to compile only Sonic Mania, nearly all of the dependencies can be ignored, so long as you have a build system properly setup.

## Windows
[Install vcpkg](https://github.com/microsoft/vcpkg#quick-start-windows), then run the following:
- `[vcpkg root]\vcpkg.exe install libtheora libogg --triplet=x64-windows-static` (the triplet can be whatever preferred)

Finally, follow the [compilation steps below](#compiling) using `-DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=(chosen triplet)` as arguments for `cmake ..`.

## Linux
Install the following dependencies: then follow the [compilation steps below](#compiling):
- **pacman (Arch):** `sudo pacman -S base-devel glew glfw libtheora portaudio`
- **apt (Debian/Ubuntu):** `sudo apt install build-essential libglew-dev libglfw3-dev libtheora-dev portaudio19-dev`
- **rpm (Fedora):** `sudo dnf install make gcc glew-devel glfw-devel libtheora-devel zlib-devel portaudio`
- Your favorite package manager here, [make a pull request](https://github.com/Rubberduckycooly/Sonic-Mania-Decompilation/fork) (also update RSDKv5U!)

#### (make sure to [install GL shaders!](#q-why-arent-videosfilters-working-while-using-gl))

## Switch
[Setup devKitPro](https://devkitpro.org/wiki/Getting_Started), then run the following:
- `(dkp-)pacman -Syuu switch-dev switch-libogg switch-libtheora switch-sdl2 switch-glad`

Finally, follow the [compilation steps below](#compiling) using `-DCMAKE_TOOLCHAIN_FILE=/opt/devkitpro/cmake/Platform/Switch.cmake` as arguments for `cmake ..`.

#### (make sure to [install GL shaders!](#q-why-arent-videosfilters-working-while-using-gl))

## Android
**Ensure you clone the repo recursively,** then follow the RSDK build instructions [here.](./dependencies/RSDKV5/dependencies/android/README.md)

### Compiling

Compiling is as simple as typing the following:
```
cmake -Bbuild # arguments go here
cmake --build build
```

The resulting build for Mania will be located somewhere in `build/` depending on your system.
If building with RSDKv5(U), the resulting Mania and RSDK executable will likely be in `build/dependencies/RSDKv5/`.

The following cmake arguments are available when compiling:
- Use these on the `cmake ..` step like so: `cmake .. -DRETRO_DISABLE_PLUS=on`

### RSDKv5 flags
- `RETRO_REVISION`: What revision to compile for. Takes an integer, defaults to `3` (RSDKv5U).
- `RETRO_DISABLE_PLUS`: Whether or not to disable the Plus DLC. Takes a boolean (on/off): build with `on` when compiling for distribution. Defaults to `off`.
- `RETRO_MOD_LOADER`: Enables or disables the mod loader. Takes a boolean, defaults to `on`.
- `RETRO_MOD_LOADER_VER`: Manually sets the mod loader version. Takes an integer, defaults to the current latest version.
- `RETRO_SUBSYSTEM`: *Only change this if you know what you're doing.* Changes the subsystem that RSDKv5 will be built for. Defaults to the most standard subsystem for the platform.   

### Sonic Mania flags
- `WITH_RSDK`= Whether or not RSDKv5 is built alongside Sonic Mania. Takes a boolean, defaults to `off`.
  - `GAME_STATIC`: Whether or not to build Sonic Mania into the resulting RSDKv5 executable. Takes a boolean, defaults change depending on the system.
- `MANIA_FIRST_RELEASE`: Whether or not to build the first release of Sonic Mania. Takes a boolean, defaults to `off`.
- `MANIA_PRE_PLUS`: Whether or not to build a pre-plus verion of Sonic Manhia. Takes a boolea, defaults to `off`.
- `GAME_VERSION`: Which release version of Sonic Mania to target for. Takes an integer, defaults to `3` when `MANIA_PRE_PLUS` is enabled, and `6` otherwise (last steam release).

### Other Platforms
The only directly supported platforms are those listed above. Since Mania is very easy to build, requiring no additional dependencies, virtually any platform that can run RSDKv5 can compile Mania easily.

However, there are a multitude of ports listed in the **[RSDKv5 repository.](https://github.com/Rubberduckycooly/RSDKv5-Decompilation)**

# FAQ
## Q: Why aren't videos/filters working while using GL?
### A: There's a mod for it that you have to make. Refer to the following directions:

Create the following directory structure inside your mods directory:
```
GLShaders/
| Data/
| | ...
| mod.ini
```

Inside `mods/GLShaders/Data/`, copy the `RSDKv5/Shaders` directory, and inside the `mod.ini`, paste this:
```
Name=GLShaders
Description=GL3 shaders
Author=Ducky
Version=1.0.0
TargetVersion=5
```

### Q: I found a bug/I have a feature request!
A: Submit an issue in the issues tab and we _might_ fix it in the main branch. Don't expect any major future releases, however.

### Q: Will you do a decompilation for Sonic CD (2011) and/or Sonic 1/2 (2013)?
A: I already have! You can find Sonic CD [here](https://github.com/Rubberduckycooly/Sonic-CD-11-Decompilation) and Sonic 1/2 [here](https://github.com/Rubberduckycooly/Sonic-1-2-2013-Decompilation).

### Q: Are there anymore decompilation projects in the works, such as Sonic Origins/Sonic 3?
A: Absolutely not. This project took about 1 and a half years to do, and doing Sonic 3 would take equally as long, if not longer, as Sonic 3 is not only larger in scope, but Origins' hybrid codebase makes it harder to read. Between our other decompilation projects and this one, we're done with decompiling, at least for the time being. We would also like to expand our horizons beyond Sonic going forward, and we don't wish to spend forever just playing catchup with Sega's official releases. Please do not expect any more decompilations from us, Sonic or otherwise!

# Special Thanks
* [st×tic](https://github.com/stxticOVFL) for leading ModAPI development, porting to other platforms, general decompilation assistance, helping me fix bugs, tweaking up my sometimes sloppy code and generally being really helpful and fun to work with on this project
* [The Weigman](https://github.com/TheWeigman) for making v5 and v5U assets with st×tic such as the header and icons
* Everyone in the [Retro Engine Modding Server](https://dc.railgun.works/retroengine) for being supportive of me and for giving me a place to show off these things that I've found

# Contact:
Join the [Retro Engine Modding Discord Server](https://dc.railgun.works/retroengine) for any extra questions you may need to know about the decompilation or modding it.
