#include "SonicMania.h"

ObjectLocalization *Localization;

void Localization_Update(void) {}

void Localization_LateUpdate(void) {}

void Localization_StaticUpdate(void) {}

void Localization_Draw(void) {}

void Localization_Create(void *data) {}

void Localization_StageLoad(void)
{
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;
    if (globals->optionsLoaded >= STATUS_OK && options->overrideLanguage) {
        Localization->language = options->language;
        Localization_LoadStrings();
    }
    else {
        Localization->language = sku_language;
        Localization_LoadStrings();
    }
}

void Localization_LoadStrings(void)
{
    RSDK.SetText(&Localization->text, "", 0);
    switch (Localization->language) {
        case LANGUAGE_EN:
            LogHelpers_Print("Loading EN strings...");
            RSDK.LoadStrings(&Localization->text, "StringsEN.txt", 16);
            break;
        case LANGUAGE_FR:
            LogHelpers_Print("Loading FR strings...");
            RSDK.LoadStrings(&Localization->text, "StringsFR.txt", 16);
            break;
        case LANGUAGE_IT:
            LogHelpers_Print("Loading IT strings...");
            RSDK.LoadStrings(&Localization->text, "StringsIT.txt", 16);
            break;
        case LANGUAGE_GE:
            LogHelpers_Print("Loading GE strings...");
            RSDK.LoadStrings(&Localization->text, "StringsGE.txt", 16);
            break;
        case LANGUAGE_SP:
            LogHelpers_Print("Loading SP strings...");
            RSDK.LoadStrings(&Localization->text, "StringsSP.txt", 16);
            break;
        case LANGUAGE_JP:
            LogHelpers_Print("Loading JP strings...");
            RSDK.LoadStrings(&Localization->text, "StringsJP.txt", 16);
            break;
#if RETRO_GAMEVER != VER_100
        case LANGUAGE_KO:
            LogHelpers_Print("Loading KO strings...");
            RSDK.LoadStrings(&Localization->text, "StringsKO.txt", 16);
            break;
        case LANGUAGE_SC:
            LogHelpers_Print("Loading Simp Chinese strings...");
            RSDK.LoadStrings(&Localization->text, "StringsSC.txt", 16);
            break;
        case LANGUAGE_TC:
            LogHelpers_Print("Loading Trad Chinese strings...");
            RSDK.LoadStrings(&Localization->text, "StringsTC.txt", 16);
            break;
#endif
        default: break;
    }
    RSDK.SplitStringList(Localization->strings, &Localization->text, 0, STR_STRING_COUNT);
    Localization->loaded = true;

#if RETRO_USE_EGS
    if (API.CheckAchievementsEnabled()) {
        TextInfo names[STR_STRING_COUNT - STR_ACHIEVEMENT];
        for (int32 i = STR_ACHIEVEMENT; i < STR_STRING_COUNT; ++i) {
            names[i] = Localization->strings[i];
        }

        API.GetAchievementNames(names, STR_STRING_COUNT - STR_ACHIEVEMENT);
    }
#endif
}

void Localization_GetString(TextInfo *textInfo, uint8 id)
{
    memset(textInfo, 0, sizeof(TextInfo));
    RSDK.SetText(textInfo, "", 0);
    RSDK.CopyString(textInfo, &Localization->strings[id]);
    for (int32 c = 0; c < textInfo->length; ++c) {
        if (textInfo->text[c] == '\\')
            textInfo->text[c] = '\n';
    }
}

void Localization_GetZoneName(TextInfo *info, uint8 zone)
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
void Localization_SetZoneNameShort(TextInfo *info, uint8 zone)
{
    switch (zone) {
        case 0: RSDK.SetText(info, "GHZ", false); break;
        case 1: RSDK.SetText(info, "CPZ", false); break;
        case 2: RSDK.SetText(info, "SPZ", false); break;
        case 3: RSDK.SetText(info, "FBZ", false); break;
        case 4: RSDK.SetText(info, "PGZ", false); break;
        case 5: RSDK.SetText(info, "SSZ", false); break;
        case 6: RSDK.SetText(info, "HCZ", false); break;
        case 7: RSDK.SetText(info, "MSZ", false); break;
        case 8: RSDK.SetText(info, "OOZ", false); break;
        case 9: RSDK.SetText(info, "LRZ", false); break;
        case 10: RSDK.SetText(info, "MMZ", false); break;
        case 11: RSDK.SetText(info, "TMZ", false); break;
        case 12: RSDK.SetText(info, "???", false); break;
        default: break;
    }
}

#if RETRO_INCLUDE_EDITOR
void Localization_EditorDraw(void) {}

void Localization_EditorLoad(void) {}
#endif

void Localization_Serialize(void) {}
