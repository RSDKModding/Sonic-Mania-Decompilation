#include "../SonicMania.h"

#if RETRO_USE_PLUS
ObjectOptions *Options;

void Options_Reload()
{
    globals->optionsRAM[23] = RSDK.GetSettingsValue(SETTINGS_SHADERID) % 4;
    globals->optionsRAM[25] = RSDK.GetSettingsValue(SETTINGS_STREAM_VOL);
    globals->optionsRAM[27] = RSDK.GetSettingsValue(SETTINGS_SFX_VOL);
    globals->optionsRAM[21] = RSDK.GetSettingsValue(SETTINGS_LANGUAGE);
    globals->optionsRAM[22] = true;
    globals->optionsRAM[29] = RSDK.GetSettingsValue(SETTINGS_VSYNC);
    globals->optionsRAM[31] = RSDK.GetSettingsValue(SETTINGS_BORDERED);
    globals->optionsRAM[32] = RSDK.GetSettingsValue(SETTINGS_WINDOWED);
    globals->optionsRAM[33] = RSDK.GetSettingsValue(SETTINGS_TRIPLEBUFFERED);
    Options_GetWinSize();
    Game_Print("optionsPtr->screenShader = %d", globals->optionsRAM[23]);
    Game_Print("optionsPtr->volMusic = %d", globals->optionsRAM[25]);
    Game_Print("optionsPtr->volSfx = %d", globals->optionsRAM[27]);
    Game_Print("optionsPtr->language = %d", globals->optionsRAM[21]);
    Game_Print("optionsPtr->overrideLanguage = %d", globals->optionsRAM[22]);
    Game_Print("optionsPtr->vsync = %d", globals->optionsRAM[29]);
    Game_Print("optionsPtr->tripleBuffering = %d", globals->optionsRAM[33]);
    Game_Print("optionsPtr->windowBorder = %d", globals->optionsRAM[31]);
    Game_Print("optionsPtr->windowed = %d", globals->optionsRAM[32]);
    Game_Print("optionsPtr->windowSize = %d", globals->optionsRAM[30]);
}

void Options_GetWinSize()
{
    int windowed = RSDK.GetSettingsValue(SETTINGS_WINDOWED);
    if (windowed == 2) {
        globals->optionsRAM[30] = 4;
    }
    else {
        int width = RSDK.GetSettingsValue(SETTINGS_WINDOW_WIDTH);
        if (width > 424) {
            if (width > 848) {
                globals->optionsRAM[30] = (width > 1272) + 2;
            }
            else {
                globals->optionsRAM[30] = 1;
            }
        }
        else {
            globals->optionsRAM[30] = 0;
        }
    }
}
#endif