#include "../SonicMania.h"

#if RETRO_USE_PLUS
ObjectOptions *Options;

void Options_Reload()
{
    options->optionsRAM[23] = RSDK.GetSettingsValue(SETTINGS_SHADERID) % 4;
    options->optionsRAM[25] = RSDK.GetSettingsValue(SETTINGS_STREAM_VOL);
    options->optionsRAM[27] = RSDK.GetSettingsValue(SETTINGS_SFX_VOL);
    options->optionsRAM[21] = RSDK.GetSettingsValue(SETTINGS_LANGUAGE);
    options->optionsRAM[22] = true;
    options->optionsRAM[29] = RSDK.GetSettingsValue(SETTINGS_VSYNC);
    options->optionsRAM[31] = RSDK.GetSettingsValue(SETTINGS_BORDERED);
    options->optionsRAM[32] = RSDK.GetSettingsValue(SETTINGS_WINDOWED);
    options->optionsRAM[33] = RSDK.GetSettingsValue(SETTINGS_TRIPLEBUFFERED);
    Options_GetWinSize();
    Game_Print("optionsPtr->screenShader = %d", options->optionsRAM[23]);
    Game_Print("optionsPtr->volMusic = %d", options->optionsRAM[25]);
    Game_Print("optionsPtr->volSfx = %d", options->optionsRAM[27]);
    Game_Print("optionsPtr->language = %d", options->optionsRAM[21]);
    Game_Print("optionsPtr->overrideLanguage = %d", options->optionsRAM[22]);
    Game_Print("optionsPtr->vsync = %d", options->optionsRAM[29]);
    Game_Print("optionsPtr->tripleBuffering = %d", options->optionsRAM[33]);
    Game_Print("optionsPtr->windowBorder = %d", options->optionsRAM[31]);
    Game_Print("optionsPtr->windowed = %d", options->optionsRAM[32]);
    Game_Print("optionsPtr->windowSize = %d", options->optionsRAM[30]);
}

void Options_GetWinSize()
{
    int windowed = RSDK.GetSettingsValue(SETTINGS_WINDOWED);
    if (windowed == 2) {
        options->optionsRAM[30] = 4;
    }
    else {
        int width = RSDK.GetSettingsValue(SETTINGS_WINDOW_WIDTH);
        if (width > 424) {
            if (width > 848) {
                options->optionsRAM[30] = (width > 1272) + 2;
            }
            else {
                options->optionsRAM[30] = 1;
            }
        }
        else {
            options->optionsRAM[30] = 0;
        }
    }
}
#endif