#include "SonicMania.h"

ObjectDASetup *DASetup;

void DASetup_Update(void) {}

void DASetup_LateUpdate(void) {}

void DASetup_StaticUpdate(void)
{
    if (!DASetup->initialized) {
        DASetup_SetupUI();
        DASetup->initialized = true;
    }

    EntityFXFade *fade = (EntityFXFade *)DASetup->fxFade;
    if (DASetup->flag && fade->timer == 512) {
        Music_FadeOut(0.02);
        RSDK.LoadScene("Presentation", "Menu");
        RSDK.InitSceneLoad();
    }

    EntityUIControl *control = (EntityUIControl *)DASetup->control;
    if (!control->childHasFocus)
        control->childHasFocus = true;
}

void DASetup_Draw(void) {}

void DASetup_Create(void *data) {}

void DASetup_StageLoad(void)
{

    foreach_all(UIControl, control)
    {
        DASetup->control          = (Entity *)control;
        control->processButtonInputCB = DASetup_State_ManageControl;
    }

    foreach_all(FXFade, fade) { DASetup->fxFade = (Entity *)fade; }

    foreach_all(UIInfoLabel, label)
    {
        if (DASetup->labelB) {
            if (!DASetup->labelA) {
                DASetup->labelA = (Entity *)label;
                foreach_break;
            }
        }
        else {
            DASetup->labelB = (Entity *)label;
        }
    }

    int trackCount = 0;
    foreach_all(Music, track) { DASetup->trackList[trackCount++] = (Entity *)track; }

    DASetup->trackCount    = trackCount;
    DASetup->activeTrack   = -1;
    DASetup->sfxEmerald    = RSDK.GetSFX("Special/Emerald.wav");
    DASetup->sfxMedal      = RSDK.GetSFX("Special/Medal.wav");
    DASetup->sfxSSExit     = RSDK.GetSFX("Special/SSExit.wav");
    DASetup->sfxScoreTotal = RSDK.GetSFX("Global/ScoreTotal.wav");
}

void DASetup_DisplayTrack(int trackID)
{
    char buffer[0x10];
    TextInfo text;
    INIT_TEXTINFO(text);

    EntityUIInfoLabel *label     = (EntityUIInfoLabel *)DASetup->labelB;
    EntityMusic *trackCountTrack = (EntityMusic *)DASetup->trackList[trackID];

    memset(buffer, 0, 0x10 * sizeof(char));
    strcpy(&buffer[2], " - ");
    buffer[0] = (trackID / 10) + '0';
    buffer[1] = trackID - 10 * (trackID / 10) + '0';
    RSDK.PrependText(&text, buffer);
    RSDK.AppendString(&text, &trackCountTrack->soundTestTitle);
    UIInfoLabel_SetString(label, &text);
}

bool32 DASetup_HandleMedallionDebug(void)
{
    int *saveRAM = NULL;
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->saveLoaded == STATUS_OK && !API.GetUserStorageNoSave()) {
            saveRAM = &globals->saveRAM[0x900];
        }
    }
    if (!globals->medallionDebug || !saveRAM)
        return false;

    switch (DASetup->trackID) {
        case 8:
            if (!SaveGame_CheckZoneClear())
                return false;
            RSDK.PlaySFX(DASetup->sfxScoreTotal, false, 255);
            return true;
        case 44:
            if (saveRAM[72] < 32) {
                SaveGame_GetMedal(globals->blueSpheresID, 1);
                SaveGame_ShuffleBSSID();
                RSDK.PlaySFX(DASetup->sfxMedal, false, 255);
            }
            else {
                if (saveRAM[71] >= 32)
                    return false;
                SaveGame_GetMedal(globals->blueSpheresID, 2);
                SaveGame_ShuffleBSSID();
                RSDK.PlaySFX(DASetup->sfxMedal, false, 255);
            }
            return true;
            break;
        case 46:
            if (saveRAM[30])
                return false;
            SaveGame_UnlockAllMedals();
            saveRAM[69] = 0;
            RSDK.PlaySFX(DASetup->sfxEmerald, false, 255);
            return true;
        case 48:
            if (saveRAM[72] <= 0 && !saveRAM[32])
                return false;
            SaveGame_ClearProgress();
            saveRAM[69] = 0;
            RSDK.PlaySFX(DASetup->sfxSSExit, false, 255);
            return true;
        default: break;
    }
    return false;
}

void DASetup_SetupUI(void)
{
    TextInfo buffer;
    INIT_TEXTINFO(buffer);

    TitleBG_SetupFX();
    DASetup_DisplayTrack(0);
    EntityUIInfoLabel *label = (EntityUIInfoLabel *)DASetup->labelA;
    Localization_GetString(&buffer, STR_SELECTATRACK);
    RSDK.PrintText(PRINT_NORMAL, &buffer);
    UIInfoLabel_SetString(label, &buffer);
}

void DASetup_State_ManageControl(void)
{
    int prevTrack = DASetup->trackID;
    if (UIControl->keyRight) {
        DASetup->trackID++;
    }
    else if (UIControl->keyLeft) {
        DASetup->trackID--;
    }
    else if (UIControl->keyUp) {
        DASetup->trackID += 10;
    }
    else if (UIControl->keyDown) {
        DASetup->trackID -= 10;
    }

    if (DASetup->trackID < 0)
        DASetup->trackID += DASetup->trackCount;
    if (DASetup->trackID >= DASetup->trackCount)
        DASetup->trackID -= DASetup->trackCount;

    if (prevTrack != DASetup->trackID) {
        RSDK.PlaySFX(UIWidgets->sfx_Bleep, false, 255);
        DASetup_DisplayTrack(DASetup->trackID);
    }

    if (UIControl->keyBack) {
        if (DASetup->activeTrack == DASetup->trackID) {
            RSDK.StopChannel(Music->channelID);
            DASetup->activeTrack = -1;
        }
        else {
            EntityMusic *track = (EntityMusic *)DASetup->trackList[DASetup->trackID];
            if (!DASetup_HandleMedallionDebug()) {
                if (track->trackFile.textLength) {
                    DASetup->activeTrack = DASetup->trackID;
                    Music_PlayTrackPtr(track);
                }
                else {
                    DASetup->activeTrack = -1;
                    RSDK.StopChannel(Music->channelID);
                }
            }
        }
    }

    if (!DASetup->flag) {
        if (UIControl->keyConfirm) {
            DASetup->flag      = true;
            EntityFXFade *fade = (EntityFXFade *)DASetup->fxFade;
            fade->state        = FXFade_State_FadeIn;
            fade->timer        = 0;
        }
    }
}

void DASetup_EditorDraw(void) {}

void DASetup_EditorLoad(void) {}

void DASetup_Serialize(void) {}
