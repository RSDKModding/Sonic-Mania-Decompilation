#include "SonicMania.h"

ObjectSoundboard *Soundboard;

void Soundboard_Update(void) {}

void Soundboard_LateUpdate(void) {}

void Soundboard_StaticUpdate(void)
{
    for (int s = 0; s < maxVal(Soundboard->sfxCount, Soundboard_SFXLimit); ++s) {
        bool32 checkFlag = true;
        if (Soundboard->sfxCheckCallback[s] && RSDK_sceneInfo->state == ENGINESTATE_REGULAR) {
            if (Soundboard->sfxCheckCallback[s]()) {
                checkFlag                  = false;
                Soundboard->sfxUnknown7[s] = 0;
                if (!Soundboard->sfxUnknown4[s]) {
                    Soundboard->sfxChannel[s]  = RSDK.PlaySfx(Soundboard->sfxList[s], Soundboard->sfxLoopPoint[s], 255);
                    Soundboard->sfxUnknown4[s] = true;
                    Soundboard->sfxUnknown6[s] = 0;
                }

                if (Soundboard->sfxUnknown8[s] > 0) {
                    RSDK.SetChannelAttributes(Soundboard->sfxUnknown8[s], 1.0, 0.0, 1.0);
                }

                if (Soundboard->sfxUpdateCallback[s]) {
                    Soundboard->sfxUpdateCallback[s](s);
                    ++Soundboard->sfxUnknown6[s];
                }
            }
        }

        if (checkFlag) {
            if (Soundboard->sfxUnknown4[s]) {
                Soundboard->sfxUnknown7[s] = 0;
                if (!Soundboard->sfxUnknown8[s])
                    RSDK.StopSFX(Soundboard->sfxList[s]);
                Soundboard->sfxUnknown4[s] = 0;
                Soundboard->sfxUnknown6[s] = 0;
            }
            else if (Soundboard->sfxUnknown8[s] > 0 && Soundboard->sfxChannel[s] > 0) {
                if (Soundboard->sfxUnknown7[s] >= Soundboard->sfxUnknown8[s]) {
                    RSDK.StopSFX(Soundboard->sfxList[s]);
                }
                else {
                    RSDK.SetChannelAttributes(Soundboard->sfxChannel[s], 1.0 - (Soundboard->sfxUnknown7[s] / (float)Soundboard->sfxUnknown8[s]), 0.0,
                                              1.0);
                    ++Soundboard->sfxUnknown7[s];
                }
            }
        }
    }
}

void Soundboard_Draw(void) {}

void Soundboard_Create(void *data) {}

void Soundboard_StageLoad(void)
{
    Soundboard->active   = ACTIVE_ALWAYS;
    Soundboard->sfxCount = 0;
    for (int i = 0; i < Soundboard_SFXLimit; ++i) {
        Soundboard->sfxList[i]           = 0;
        Soundboard->sfxLoopPoint[i]      = 0;
        Soundboard->sfxCheckCallback[i]  = NULL;
        Soundboard->sfxUpdateCallback[i] = NULL;
        Soundboard->sfxUnknown4[i]       = 0;
        Soundboard->sfxUnknown6[i]       = 0;
        Soundboard->sfxUnknown8[i]       = 0;
    }
}

uint8 Soundboard_LoadSFX(const char *sfxName, uint32 loopPoint, bool32 (*checkCallback)(void), void (*updateCallback)(int))
{
    if (!Soundboard)
        return -1;
    if (Soundboard->sfxCount >= Soundboard_SFXLimit)
        return -1;
    int sfxID = Soundboard->sfxCount;

    Soundboard->sfxList[sfxID] = RSDK.GetSFX(sfxName);
    int loop                   = 1;
    if (loopPoint >= 1)
        loop = loopPoint;
    Soundboard->sfxLoopPoint[sfxID]      = loopPoint;
    Soundboard->sfxCheckCallback[sfxID]  = checkCallback;
    Soundboard->sfxUpdateCallback[sfxID] = updateCallback;
    Soundboard->sfxUnknown8[sfxID]       = 0;
    RSDK.StopSFX(Soundboard->sfxList[sfxID]);
    ++Soundboard->sfxCount;
    return sfxID;
}

void Soundboard_EditorDraw(void) {}

void Soundboard_EditorLoad(void) {}

void Soundboard_Serialize(void) {}
