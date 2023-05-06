// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DASetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDASetup *DASetup;

void DASetup_Update(void) {}

void DASetup_LateUpdate(void) {}

void DASetup_StaticUpdate(void)
{
    if (!DASetup->initialized) {
        DASetup_SetupUI();
        DASetup->initialized = true;
    }

    EntityFXFade *fade = DASetup->fxFade;
    if (DASetup->returnToMenu && fade->timer == 512) {
        Music_FadeOut(0.02);
        RSDK.SetScene("Presentation", "Menu");
        RSDK.LoadScene();
    }

    EntityUIControl *control = DASetup->control;
    if (!control->childHasFocus)
        control->childHasFocus = true;
}

void DASetup_Draw(void) {}

void DASetup_Create(void *data) {}

void DASetup_StageLoad(void)
{

    foreach_all(UIControl, control)
    {
        DASetup->control              = control;
        control->processButtonInputCB = DASetup_State_ManageControl;
    }

    foreach_all(FXFade, fade) { DASetup->fxFade = fade; }

    foreach_all(UIInfoLabel, label)
    {
        if (DASetup->trackTitleLabel) {
            if (!DASetup->trackSelLabel) {
                DASetup->trackSelLabel = label;
                foreach_break;
            }
        }
        else {
            DASetup->trackTitleLabel = label;
        }
    }

    int32 trackCount = 0;
    foreach_all(Music, track) { DASetup->trackList[trackCount++] = track; }

    DASetup->trackCount  = trackCount;
    DASetup->activeTrack = TRACK_NONE;

    DASetup->sfxEmerald    = RSDK.GetSfx("Special/Emerald.wav");
    DASetup->sfxMedal      = RSDK.GetSfx("Special/Medal.wav");
    DASetup->sfxSSExit     = RSDK.GetSfx("Special/SSExit.wav");
    DASetup->sfxScoreTotal = RSDK.GetSfx("Global/ScoreTotal.wav");
}

void DASetup_DisplayTrack(int32 trackID)
{
    char buffer[0x10];
    String text;
    INIT_STRING(text);

    EntityUIInfoLabel *trackTitleLabel = DASetup->trackTitleLabel;
    EntityMusic *trackCountTrack       = DASetup->trackList[trackID];

    memset(buffer, 0, 0x10 * sizeof(char));
    strcpy(&buffer[2], " - ");
    buffer[0] = (trackID / 10) + '0';
    buffer[1] = trackID - 10 * (trackID / 10) + '0';
    RSDK.SetString(&text, buffer);
    RSDK.AppendString(&text, &trackCountTrack->soundTestTitle);
    UIInfoLabel_SetString(trackTitleLabel, &text);
}

bool32 DASetup_HandleMedallionDebug(void)
{
    ProgressRAM *progress = GameProgress_GetProgressRAM();

    if (globals->medallionDebug && progress) {
        switch (DASetup->trackID) {
            case 8: // Act Clear/1UP - unlock all zones
                if (GameProgress_CheckZoneClear()) {
                    RSDK.PlaySfx(DASetup->sfxScoreTotal, false, 255);
                    return true;
                }
                break;

            case 44: // Blue Spheres/ERZ Pinch Mode - unlock all medals (silver first if not all unlocked, if all unlocked then gold)
                if (progress->silverMedalCount < 32) {
                    GameProgress_GiveMedal(globals->blueSpheresID, 1);
                    GameProgress_ShuffleBSSID();
                    RSDK.PlaySfx(DASetup->sfxMedal, false, 255);
                    return true;
                }
                else if (progress->goldMedalCount < 32) {
                    GameProgress_GiveMedal(globals->blueSpheresID, 2);
                    GameProgress_ShuffleBSSID();
                    RSDK.PlaySfx(DASetup->sfxMedal, false, 255);
                    return true;
                }
                break;

            case 46: // Super/Blue Spheres - unlock all
                if (!progress->allGoldMedals) {
                    GameProgress_UnlockAll();
                    GameProgress_LockAllSpecialClear();
                    RSDK.PlaySfx(DASetup->sfxEmerald, false, 255);
                    return true;
                }
                break;

            case 48: // Game Over/Credits - reset progress
                if (progress->silverMedalCount > 0 || progress->zoneCleared[0]) {
                    GameProgress_ClearProgress();
                    progress->allSpecialCleared = false;
                    RSDK.PlaySfx(DASetup->sfxSSExit, false, 255);
                    return true;
                }
                break;

            default: break;
        }
    }

    return false;
}

void DASetup_SetupUI(void)
{
    String buffer;
    INIT_STRING(buffer);

    TitleBG_SetupFX();
    DASetup_DisplayTrack(0);
    EntityUIInfoLabel *trackSelLabel = DASetup->trackSelLabel;
    Localization_GetString(&buffer, STR_SELECTATRACK);
#if MANIA_USE_PLUS
    LogHelpers_PrintString(&buffer);
#endif
    UIInfoLabel_SetString(trackSelLabel, &buffer);
}

void DASetup_State_ManageControl(void)
{
    int32 prevTrack = DASetup->trackID;
    if (UIControl->anyRightPress)
        DASetup->trackID++;
    else if (UIControl->anyLeftPress)
        DASetup->trackID--;
    else if (UIControl->anyUpPress)
        DASetup->trackID += 10;
    else if (UIControl->anyDownPress)
        DASetup->trackID -= 10;

    if (DASetup->trackID < 0)
        DASetup->trackID += DASetup->trackCount;
    if (DASetup->trackID >= DASetup->trackCount)
        DASetup->trackID -= DASetup->trackCount;

    if (prevTrack != DASetup->trackID) {
        RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
        DASetup_DisplayTrack(DASetup->trackID);
    }

    if (UIControl->anyConfirmPress) {
        if (DASetup->activeTrack == DASetup->trackID) {
            Music_Stop();
            DASetup->activeTrack = TRACK_NONE;
        }
        else {
            EntityMusic *track = DASetup->trackList[DASetup->trackID];
            if (!DASetup_HandleMedallionDebug()) {
                if (track->trackFile.length) {
                    DASetup->activeTrack = DASetup->trackID;
                    Music_PlayTrackPtr(track);
                }
                else {
                    DASetup->activeTrack = TRACK_NONE;
                    Music_Stop();
                }
            }
        }
    }

    if (!DASetup->returnToMenu && UIControl->anyBackPress) {
        DASetup->returnToMenu = true;

        EntityFXFade *fade = DASetup->fxFade;
        fade->state        = FXFade_State_FadeOut;
        fade->timer        = 0;
    }
}

#if GAME_INCLUDE_EDITOR
void DASetup_EditorDraw(void) {}

void DASetup_EditorLoad(void) {}
#endif

void DASetup_Serialize(void) {}
