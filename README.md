# **SUPPORT THE OFFICIAL RELEASE OF SONIC MANIA (PLUS)**
+ Without assets from the official releases, this decompilation will not run.

+ You can get the official release of Sonic Mania (Plus) from:
  * [Windows]
    * [Via Steam](https://store.steampowered.com/app/584400/Sonic_Mania/)
    * [Via The Epic Games Store](https://www.epicgames.com/store/en-US/p/sonic-mania)
    * [Via Origin](https://www.origin.com/aus/en-us/store/sonic-the-hedgehog/sonic-mania)
  * [Switch, via the eShop](https://www.nintendo.com/games/detail/sonic-mania-switch/)
  * [PS4, via the Store](https://store.playstation.com/en-us/product/UP0177-CUSA07023_00-SONICMANIA000000)
  * [Xbox One, via the Store](https://www.xbox.com/en-US/games/store/sonic-mania/BXH46NQT9W4Q/0001)

Even if your platform isn't supported by the official releases, you **must** buy or officially download it for the assets.

## **DO NOT USE THIS DECOMPILATION PROJECT AS A MEANS TO PIRATE SONIC MANIA.**
We do not condone using this project as a means for piracy in any form. This project was made with love and care for the source material and was created for purely educational purposes, and would not exist without the work of Sega, Headcannon, and Evening Star.

If you want to transfer your save from the official PC versions, you can just copy your savedata into the folder containing the decompilation!

# Additional Tweaks
* Added a built-in mod loader and API calls to enable a much smoother modding experience.
* Added support for targeting RSDKv5U rather than standalone RSDKv5
* Added all content from all released versions of the game. Including: 1.00 (Console initial release), 1.03 (PC initial release) & 1.06 (Plus update)

# How to Build
Most platforms will heavily encourage you to build it in conjunction with RSDKv5. However, there are some options available for some platforms if you wish to build separately.

## Windows
Open SonicMania.sln and build the project you wish to build.
* Projects ending with `All` use the generated `All.c` file in-place of compiling each object seperately. Use for faster compilation speed.
  * Adversely, projects without `All` compile each object using their separate source files. Use for JIT-compiling.
* The version you choose **must match up** with the RSDKv5 version you run. A v5U executable cannot run a v5 Mania, etc.

## Linux/Make-like systems
The makefile is a trimmed down version of the RSDKv5 makefile that only supports game compilation.
By default, unlike the RSDK makefile, this will look for the default game name of `SonicMania` and use it as source and compile as such. Pass `GAME_NAME` to the makefile to change it/

### Other platforms
The only directly supported platforms are those listed above. Since Mania is very easy to build, requiring no dependencies, virtually any platform that can run RSDKv5 can compile Mania easily.

# FAQ
### Q: I found a bug/I have a feature request!
A: Submit an issue in the issues tab and we _might_ fix it in the main branch. Don't expect any major future releases, however.

### Q: Will you do a decompilation for Sonic CD (2011) and/or Sonic 1/2 (2013)?
A: I already have! You can find Sonic CD [here](https://github.com/Rubberduckycooly/Sonic-CD-11-Decompilation) and Sonic 1/2 [here](https://github.com/Rubberduckycooly/Sonic-1-2-2013-Decompilation).

### Q: Will there be a decompilation for Sonic Origins/Sonic 3?
A: No. This is the last decompilation from us. This project took about 1.5 years to do, and doing Sonic 3 would take equally as long, if not longer, as Sonic 3 is not only larger in scope, but Origins' hybrid codebase makes it harder to read. We would also like to expand our horizons beyond sonic going forward, and we don't wish to spend forever just playing catchup with Sega's official releases.

### Q: Are there anymore decompilation projects in the works?
A: Absolutely not. Between the last two and this one, we're done with decompiling, at least for the time being. Please do not expect any more decompilations from us, Sonic or otherwise!

# Special Thanks
* [Chuli](https://github.com/MGRich) for general decompilation assistance, helping me fix bugs, tweaking up my sometimes sloppy code and generally being really helpful and fun to work with on this project
* The Weigman for creating the header you see up here along with similar assets
* Everyone in the [Retro Engine Modding Server](https://dc.railgun.works/retroengine) for being supportive of me and for giving me a place to show off these things that I've found

# Contact:
Join the [Retro Engine Modding Discord Server](https://dc.railgun.works/retroengine) for any extra questions you may need to know about the decompilation or modding it.
