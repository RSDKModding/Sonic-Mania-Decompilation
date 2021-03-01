#include "../SonicMania.hpp"

ObjectSoundboard *Soundboard;

void Soundboard_Update()
{

}

void Soundboard_LateUpdate()
{

}

void Soundboard_StaticUpdate()
{
    for (int s = 0; s < maxVal(Soundboard->sfxCount, 0x20); ++s) {
        int (*checkCallback)(void)  = (int (*)(void))Soundboard->sfxCheckCallback[s];
        void (*changeCallback)(int) = (void(__cdecl *)(int))Soundboard->sfxChangeCallback[s];

        if (checkCallback && RSDK_sceneInfo->state == ENGINESTATE_REGULAR) {
            if (checkCallback()) {
                Soundboard->sfxUnknown7[s] = 0;
                if (!Soundboard->sfxUnknown4[s]) {
                    Soundboard->sfxUnknownVal[s] = RSDK.PlaySFX(Soundboard->sfxList[s], Soundboard->sfxLoopPoint[s], 255);
                    Soundboard->sfxUnknown4[s]   = true;
                    Soundboard->sfxUnknown6[s]   = 0;
                }

                if (Soundboard->sfxUnknown8[s] > 0) {
                    RSDK.SetSoundAttributes(0, Soundboard->sfxUnknown8[s], 1.0, 0.0, 1.0);
                }

                if (changeCallback) {
                    changeCallback(s);
                    ++Soundboard->sfxUnknown6[s];
                }
            }
        }
        else if (Soundboard->sfxUnknown4[s]) {
            Soundboard->sfxUnknown7[s] = 0;
            if (!Soundboard->sfxUnknown8[s])
                RSDK.StopSFX(Soundboard->sfxList[s]);
            Soundboard->sfxUnknown4[s] = 0;
            Soundboard->sfxUnknown6[s] = 0;
        }
        else if (Soundboard->sfxUnknown8[s] > 0 && Soundboard->sfxUnknownVal[s] > 0) {
            if (Soundboard->sfxUnknown7[s] >= Soundboard->sfxUnknown8[s]) {
                RSDK.StopSFX(Soundboard->sfxList[s]);
            }
            else {
                RSDK.SetSoundAttributes(0, Soundboard->sfxUnknownVal[s],
                                        1.0 - (float)((float)Soundboard->sfxUnknown7[s] / (float)Soundboard->sfxUnknown8[s]), 0.0, 1.0);
                ++Soundboard->sfxUnknown7[s];
            }
        }
    }
}

void Soundboard_Draw()
{

}

void Soundboard_Create(void* data)
{

}

void Soundboard_StageLoad()
{
    Soundboard->active   = ACTIVE_ALWAYS;
    Soundboard->sfxCount = 0;
    for (int i = 0; i < 0x20; ++i) {
        Soundboard->sfxList[i]           = 0;
        Soundboard->sfxLoopPoint[i]       = 0;
        Soundboard->sfxCheckCallback[i]  = NULL;
        Soundboard->sfxChangeCallback[i] = NULL;
        Soundboard->sfxUnknown4[i]       = 0;
        Soundboard->sfxUnknown6[i]       = 0;
        Soundboard->sfxUnknown8[i]       = 0;

        //Soundboard->sfxUnknownVal[i]     = 0;
        //Soundboard->sfxUnknown7[i]       = 0;
    }
}

byte Soundboard_LoadSFX(const char *sfxName, uint loopPoint, void *checkCallback, void *changeCallback)
{
    if (!Soundboard)
        return -1;
    if (Soundboard->sfxCount >= 32)
        return -1;
    int sfxID = Soundboard->sfxCount;

    Soundboard->sfxList[sfxID] = RSDK.GetSFX(sfxName);
    int loop                   = 1;
    if (loopPoint >= 1)
        loop = loopPoint;
    Soundboard->sfxLoopPoint[sfxID]      = loopPoint;
    Soundboard->sfxCheckCallback[sfxID]  = checkCallback;
    Soundboard->sfxChangeCallback[sfxID] = changeCallback;
    Soundboard->sfxUnknown8[sfxID]       = 0;
    RSDK.StopSFX(Soundboard->sfxList[sfxID]);
    ++Soundboard->sfxCount;
    return sfxID;
}

void Soundboard_EditorDraw()
{

}

void Soundboard_EditorLoad()
{

}

void Soundboard_Serialize()
{

}

