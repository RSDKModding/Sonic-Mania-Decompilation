#include "../SonicMania.h"

ObjectLocalization *Localization;

void Localization_Update() {}

void Localization_LateUpdate() {}

void Localization_StaticUpdate() {}

void Localization_Draw() {}

void Localization_Create(void *data) {}

void Localization_StageLoad()
{
    if (options->optionsLoaded >= 200 && options->optionsRAM[22]) {
        Localization->language = (options->optionsRAM[21] >> 8) & 0xFF;
        Localization_LoadStrings();
    }
    else {
        Localization->language = RSDK_sku->language;
        Localization_LoadStrings();
    }
}

void Localization_LoadStrings()
{
    RSDK.SetText(&Localization->text, "", 0);
    switch (Localization->language) {
        case LANGUAGE_EN:
            Game_Print("Loading EN strings...");
            RSDK.LoadStrings(&Localization->text, "StringsEN.txt", 16);
            break;
        case LANGUAGE_FR:
            Game_Print("Loading FR strings...");
            RSDK.LoadStrings(&Localization->text, "StringsFR.txt", 16);
            break;
        case LANGUAGE_IT:
            Game_Print("Loading IT strings...");
            RSDK.LoadStrings(&Localization->text, "StringsIT.txt", 16);
            break;
        case LANGUAGE_GE:
            Game_Print("Loading GE strings...");
            RSDK.LoadStrings(&Localization->text, "StringsGE.txt", 16);
            break;
        case LANGUAGE_SP:
            Game_Print("Loading SP strings...");
            RSDK.LoadStrings(&Localization->text, "StringsSP.txt", 16);
            break;
        case LANGUAGE_JP:
            Game_Print("Loading JP strings...");
            RSDK.LoadStrings(&Localization->text, "StringsJP.txt", 16);
            break;
        case LANGUAGE_KO:
            Game_Print("Loading KO strings...");
            RSDK.LoadStrings(&Localization->text, "StringsKO.txt", 16);
            break;
        case LANGUAGE_SC:
            Game_Print("Loading Simp Chinese strings...");
            RSDK.LoadStrings(&Localization->text, "StringsSC.txt", 16);
            break;
        case LANGUAGE_TC:
            Game_Print("Loading Trad Chinese strings...");
            RSDK.LoadStrings(&Localization->text, "StringsTC.txt", 16);
            break;
        default: break;
    }
    RSDK.Unknown68(Localization->strings, &Localization->text, 0, 68);
    Localization->loaded = true;
}

void Localization_GetString(TextInfo *textInfo, byte id)
{
    RSDK.SetText(textInfo, "", 0);
    RSDK.Unknown64(textInfo, &Localization->strings[id]);
    for (int c = 0; c < textInfo->textLength; ++c) {
        if (textInfo->text[c] == '\\')
            textInfo->text[c] = '\n';
    }
}

void Localization_GetZoneName(TextInfo *info, byte zone)
{
    switch (zone) {
        case 0: RSDK.SetText(info, "GREEN HILL", 0); break;
        case 1: RSDK.SetText(info, "CHEMICAL PLANT", 0); break;
        case 2: RSDK.SetText(info, "STUDIOPOLIS", 0); break;
        case 3: RSDK.SetText(info, "FLYING BATTERY", 0); break;
        case 4: RSDK.SetText(info, "PRESS GARDEN", 0); break;
        case 5: RSDK.SetText(info, "STARDUST SPEEDWAY", 0); break;
        case 6: RSDK.SetText(info, "HYDROCITY", 0); break;
        case 7: RSDK.SetText(info, "MIRAGE SALOON", 0); break;
        case 8: RSDK.SetText(info, "OIL OCEAN", 0); break;
        case 9: RSDK.SetText(info, "LAVA REEF", 0); break;
        case 10: RSDK.SetText(info, "METALLIC MADNESS", 0); break;
        case 11: RSDK.SetText(info, "TITANIC MONARCH", 0); break;
        case 12: RSDK.SetText(info, "???", 0); break;
        default: break;
    }
}
void Localization_SetZoneNameShort(TextInfo *info, byte zone)
{
    switch (zone) {
        case 0: RSDK.SetText(info, "GHZ", 0); break;
        case 1: RSDK.SetText(info, "CPZ", 0); break;
        case 2: RSDK.SetText(info, "SPZ", 0); break;
        case 3: RSDK.SetText(info, "FBZ", 0); break;
        case 4: RSDK.SetText(info, "PGZ", 0); break;
        case 5: RSDK.SetText(info, "SSZ", 0); break;
        case 6: RSDK.SetText(info, "HCZ", 0); break;
        case 7: RSDK.SetText(info, "MSZ", 0); break;
        case 8: RSDK.SetText(info, "OOZ", 0); break;
        case 9: RSDK.SetText(info, "LRZ", 0); break;
        case 10: RSDK.SetText(info, "MMZ", 0); break;
        case 11: RSDK.SetText(info, "TMZ", 0); break;
        case 12: RSDK.SetText(info, "???", 0); break;
        default: break;
    }
}

void Localization_EditorDraw() {}

void Localization_EditorLoad() {}

void Localization_Serialize() {}
