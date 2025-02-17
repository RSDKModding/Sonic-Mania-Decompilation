# **SUPPORT THE OFFICIAL RELEASE OF SONIC MANIA (PLUS)**
+ Without assets from the official releases, this decompilation will not run.

+ You can get the official release of Sonic Mania (Plus) from:
  * Windows
    * [Via Steam](https://store.steampowered.com/app/584400/Sonic_Mania/)
    * [Via The Epic Games Store](https://www.epicgames.com/store/en-US/p/sonic-mania)
  * [Switch, via the eShop](https://www.nintendo.com/games/detail/sonic-mania-switch/)
  * [PS4, via the Store](https://store.playstation.com/en-us/product/UP0177-CUSA07023_00-SONICMANIA000000)
  * [Xbox One, via the Store](https://www.xbox.com/en-US/games/store/sonic-mania/BXH46NQT9W4Q/0001)

Even if your platform isn't supported by the above official releases, you **must** buy it for the assets. The Netflix Games version is NOT supported.

## **DO NOT USE THIS DECOMPILATION PROJECT AS A MEANS TO PIRATE SONIC MANIA (PLUS)**
We do not condone using this project as a means for piracy in any form. This project was made with love and care for the source material and was created for purely educational purposes, and would not exist without the work of Sega, Headcannon, and Evening Star.

If you want to transfer your save from the official PC versions, you can just copy your savedata into the folder containing the decompilation!

# Additional Tweaks
* Added a built-in mod loader and API calls to enable a much smoother modding experience.
* Added support for targeting RSDKv5U rather than standalone RSDKv5
* Added all content from almost all released versions of the game. Including: 1.00 (Console initial release), 1.03 (PC initial release) & 1.06 (Plus update)

# How to Build

This project uses [CMake](https://cmake.org/), a versatile building system that supports many different compilers and platforms. You can download CMake [here](https://cmake.org/download/). **(Make sure to enable the feature to add CMake to the system PATH during the installation if you're on Windows!)**

## Get the source code

In order to clone the repository, you need to install Git, which you can get [here](https://git-scm.com/downloads).

Clone the repo **recursively**, using:
`git clone --recursive https://github.com/RSDKModding/Sonic-Mania-Decompilation`

If you've already cloned the repo, run this command inside of the repository:
```git submodule update --init --recursive```

## Getting dependencies

This repo includes everything you need for Sonic Mania + RSDKv5(U). If you wish to compile only Sonic Mania, you can skip to the [compilation steps below](#compiling).

### Windows
To handle dependencies, you'll need to install [Visual Studio Community](https://visualstudio.microsoft.com/downloads/) (make sure to install the `Desktop development with C++` package during the installation) and [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-cmd#1---set-up-vcpkg) (You only need to follow `1 - Set up vcpkg`).

After installing those, run the following in Command Prompt (make sure to replace `[vcpkg root]` with the path to the vcpkg installation!):
- `[vcpkg root]/vcpkg.exe install libtheora libogg glew glfw3 sdl2 --triplet=x64-windows-static` (If you're compiling a 32-bit build, replace `x64-windows-static` with `x86-windows-static`.)

Finally, follow the [compilation steps below](#compiling) using `-DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static` as arguments for `cmake -B build`.
  - Make sure to replace `[vcpkg root]` with the path to the vcpkg installation!
  - If you're compiling a 32-bit build, replace `x64-windows-static` with `x86-windows-static`.

### Linux
Install the following dependencies: then follow the [compilation steps below](#compiling):
- **pacman (Arch):** `sudo pacman -S base-devel cmake glew glfw libtheora`
- **apt (Debian/Ubuntu):** `sudo apt install build-essential cmake libglew-dev libglfw3-dev libtheora-dev`
- **rpm (Fedora):** `sudo dnf install make cmake gcc glew-devel glfw-devel libtheora-devel zlib-devel`
- **xbps (Void):** `sudo xbps-install make cmake gcc pkg-config glew-devel glfw-devel libtheora-devel zlib-devel`
- Your favorite package manager here, [make a pull request](https://github.com/RSDKModding/Sonic-Mania-Decompilation/fork) (also update [RSDKv5U](https://github.com/RSDKModding/RSDKv5-Decompilation)!)

#### (make sure to [install GL shaders!](#q-why-arent-videosfilters-working-while-using-gl))

### Switch
[Setup devKitPro](https://devkitpro.org/wiki/Getting_Started), then run the following:
- `(dkp-)pacman -Syuu switch-dev switch-libogg switch-libtheora switch-sdl2 switch-glad`

Finally, follow the [compilation steps below](#compiling) using `-DCMAKE_TOOLCHAIN_FILE=/opt/devkitpro/cmake/Switch.cmake` as arguments for `cmake -B build`.

#### (make sure to [install GL shaders!](#q-why-arent-videosfilters-working-while-using-gl))

### Android
**Ensure you clone the repo recursively,** then follow the RSDK build instructions [here.](https://github.com/RSDKModding/RSDKv5-Decompilation/blob/master/dependencies/android/README.md)

## Compiling

Compiling is as simple as typing the following in the root repository directory:
```
cmake -B build
cmake --build build --config release
```

The resulting build for Mania will be located somewhere in `build/` depending on your system.
If building with RSDKv5(U), the resulting Mania and RSDK executable will likely be in `build/dependencies/RSDKv5/`.

The following cmake arguments are available when compiling:
- Use these by adding `-D[flag-name]=[value]` to the end of the `cmake -B build` command. For example, to build with `RETRO_DISABLE_PLUS` set to on, add `-DRETRO_DISABLE_PLUS=on` to the command.

### RSDKv5 flags
- `RETRO_REVISION`: What revision to compile for. Takes an integer, defaults to `3` (RSDKv5U).
- `RETRO_DISABLE_PLUS`: Whether or not to disable the Plus DLC. Takes a boolean (on/off): build with `on` when compiling for distribution. Defaults to `off`.
- `RETRO_MOD_LOADER`: Enables or disables the mod loader. Takes a boolean, defaults to `on`.
- `RETRO_MOD_LOADER_VER`: Manually sets the mod loader version. Takes an integer, defaults to the current latest version.
- `RETRO_SUBSYSTEM`: *Only change this if you know what you're doing.* Changes the subsystem that RSDKv5 will be built for. Defaults to the most standard subsystem for the platform.

### Sonic Mania flags
- `WITH_RSDK`: Whether or not RSDKv5 is built alongside Sonic Mania. Takes a boolean, defaults to `on`.
  - `GAME_STATIC`: Whether or not to build Sonic Mania into the resulting RSDKv5 executable. Takes a boolean, defaults change depending on the system.
- `MANIA_FIRST_RELEASE`: Whether or not to build the first console release of Sonic Mania. Takes a boolean, defaults to `off`.
- `MANIA_PRE_PLUS`: Whether or not to build a pre-plus version of Sonic Mania. Takes a boolean, defaults to `off`.
- `GAME_INCLUDE_EDITOR`: Whether or not to include functions for use in certain RSDKv5 scene editors. Takes a boolean, defaults to `on`.
- `GAME_VERSION`: Which release version of Sonic Mania to target for. Takes an integer, defaults to `3` when `MANIA_PRE_PLUS` is enabled, and `6` otherwise (last steam release).

### Other Platforms
The only directly supported platforms are those listed above. Since Mania is very easy to build, requiring no additional dependencies, virtually any platform that can run RSDKv5 can compile Mania easily.

However, there are a multitude of ports listed in the **[RSDKv5 repository.](https://github.com/RSDKModding/RSDKv5-Decompilation)**

# FAQ
### Q: Why aren't videos/filters working while using GL?
A: There's a mod for it that you have to make. Refer to the following directions:

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

### Q: I found a bug!
A: Submit an issue in the Issues tab and we might look into it. Keep in mind that this is a decompilation, so bugs that exist in official releases will most likely not be fixed here.

### Q: Will you add support for the Netflix Games version of Mania?
A: No. There are several reasons for this: the APIs introduced in the port that we'd have to deal with, the pressure of having to maintain support for a version that's being frequently updated, concerns over piracy, and a general disinterest in the port. If you want to play the decomp on mobile with touch controls, there's a [mod](https://github.com/RSDKModding/RSDKv5-Example-Mods/tree/master/ManiaTouchControls) for that you can use instead.

### Q: Will you do a decompilation for Sonic CD (2011) and/or Sonic 1/2 (2013)?
A: I already have! You can find Sonic CD [here](https://github.com/RSDKModding/RSDKv3-Decompilation) and Sonic 1/2 [here](https://github.com/RSDKModding/RSDKv4-Decompilation).

### Q: Are there anymore decompilation projects in the works, such as Sonic Origins/Sonic 3 & Knuckles?
A: Absolutely not. This project took about 1 and a half years to do, and doing Sonic 3 & Knuckles would take equally as long, if not longer, as it's not only larger in scope, but Origins' hybrid codebase makes it harder to read. Between our other decompilation projects and this one, we're done with decompiling, at least for the time being. We would also like to expand our horizons beyond Sonic going forward, and we don't wish to spend forever just playing catchup with Sega's official releases. Please do not expect any more decompilations from us, Sonic or otherwise!

# Special Thanks
* [st×tic](https://github.com/stxticOVFL) for leading ModAPI development, porting to other platforms, general decompilation assistance, helping me fix bugs, tweaking up my sometimes sloppy code and generally being really helpful and fun to work with on this project
* [The Weigman](https://github.com/TheWeigman) for making v5 and v5U assets with st×tic such as the header and icons
* Everyone in the [Retro Engine Modding Server](https://dc.railgun.works/retroengine) for being supportive of me and for giving me a place to show off these things that I've found

# Contact:
Join the [Retro Engine Modding Discord Server](https://dc.railgun.works/retroengine) for any extra questions you may need to know about the decompilation or modding it.
