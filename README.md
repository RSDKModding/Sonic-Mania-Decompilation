![](header.png?raw=true)
# **SUPPORT THE OFFICIAL RELEASE OF SONIC MANIA (PLUS)**
+ Without assets from the official releases, this decompilation will not run.

+ You can get the official release of Sonic Mania (Plus) from:
  * [Windows, Via Steam](https://store.steampowered.com/app/584400/Sonic_Mania/)
  * [Windows, Via the Epic Games Store](https://www.epicgames.com/store/en-US/p/sonic-mania)
  * [Windows, Via Origin](https://www.origin.com/aus/en-us/store/sonic-the-hedgehog/sonic-mania)
  * [Switch, Via the eshop](https://www.nintendo.com/games/detail/sonic-mania-switch/)
  * [PS4, Via the store](https://store.playstation.com/en-au/product/EP0177-CUSA07010_00-SONICMANIA000000/)
  * [XBox One, Via the store](https://www.xbox.com/en-au/games/store/Sonic-Mania/BXH46NQT9W4Q)

Even if your platform isn't supported by the official releases, you **must** buy or officially download it for the assets (you don't need to run the official release, you just need the game assets)

## **DO NOT USE THIS DECOMPILATION PROJECT AS A MEANS TO PIRATE SONIC MANIA.**
We do not condone using this project as a means for piracy in any form, this project was made with love and care for the source material and was created for purely educational purposes.

If you want to transfer your save from the official PC versions, you can just copy your savedata into the folder containing the decompilation!

# Additional Tweaks
* Added a built in mod loader and API allowing to easily create and play mods with features such as save file redirection and XML Asset loading.

# How to build
## Windows
* Clone the repo, then follow the instructions in the [depencencies readme for Windows](./dependencies/windows/dependencies.txt) to setup dependencies, then build via the visual studio solution (or grab a prebuilt executable from the releases section.)

## Linux:
* TODO LOLLL

## Mac
* Clone the repo, follow the instructions in the [depencencies readme for Mac](./dependencies/mac/dependencies.txt) to setup dependencies, then build via the Xcode project.

## Android
* Clone the repo, then follow the instructions in the [depencencies readme for Android](./dependencies/android/dependencies.txt).
* Ensure the symbolic links in `android/app/jni` are correct. If not, fix them with the following on Windows:
  * `mklink /D src ..\..\..`
  * `mklink /D SDL ..\..\..\dependencies\android\SDL`
* Open `android/` in Android Studio, install the NDK and everything else that it asks for, and build.

## Other platforms
Currently the only supported platforms are the ones listed above, however the backend uses libogg, libvorbis & SDL2 to power it, so the codebase is very multiplatform.
If you've cloned this repo and ported it to a platform not on the list or made some changes you'd like to see added to this repo, submit a pull request and it'll most likely be added.

# FAQ
### Q: The screen is tearing, how do I fix it?
A: Try turning on VSync in settings.ini.

### Q: I found a bug/I have a feature request!
A: Submit an issue in the issues tab and we _might_ fix it in the main branch. Don't expect any future releases, however.

### Q: Will you do a decompilation for Sonic CD (2011)?
A: I already have! You can find it [here](https://github.com/Rubberduckycooly/Sonic-CD-11-Decompilation).

### Q: Will you do a decompilation for Sonic 1 or Sonic 2 (2013)?
A: I already have! You can find it [here](https://github.com/Rubberduckycooly/Sonic-1-2-2013-Decompilation).

### Q: Do you have anymore decompilation projects in the works?
A: Absolutely not. Between the last two and this one, I'm done with decompiling, at least for the time being. Please do not expect any more decompilations from us, sonic or otherwise!

# Special Thanks
* [Lulu](https://github.com/MGRich) for general decompilation assistance, helping me fix bugs, tweaking up my sometimes sloppy code and generally being really helpful and fun to work with on this project
* The Weigman for creating the header you see up here along with similar assets
* Everyone in the [Retro Engine Modding Server](https://dc.railgun.works/retroengine) for being supportive of me and for giving me a place to show off these things that I've found

# Contact:
Join the [Retro Engine Modding Discord Server](https://dc.railgun.works/retroengine) for any extra questions you may need to know about the decompilation or modding it.
