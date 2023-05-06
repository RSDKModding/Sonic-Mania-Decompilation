#ifndef OBJ_SSZ1OUTRO_H
#define OBJ_SSZ1OUTRO_H

#include "Game.h"

// Object Class
struct ObjectSSZ1Outro {
    RSDK_OBJECT
    int32 unused;
    EntityFXFade *fxFade;
    EntityRTeleporter *teleporter;
};

// Entity Class
struct EntitySSZ1Outro {
    MANIA_CUTSCENE_BASE
};

// Object Struct
extern ObjectSSZ1Outro *SSZ1Outro;

// Standard Entity Events
void SSZ1Outro_Update(void);
void SSZ1Outro_LateUpdate(void);
void SSZ1Outro_StaticUpdate(void);
void SSZ1Outro_Draw(void);
void SSZ1Outro_Create(void *data);
void SSZ1Outro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SSZ1Outro_EditorDraw(void);
void SSZ1Outro_EditorLoad(void);
#endif
void SSZ1Outro_Serialize(void);

// Extra Entity Functions
bool32 SSZ1Outro_Cutscene_TimeWarpRunway(EntityCutsceneSeq *host);
bool32 SSZ1Outro_Cutscene_TimeWarp(EntityCutsceneSeq *host);

void SSZ1Outro_DestroyHotaru(EntityHotaruMKII *hotaru);
void SSZ1Outro_Trigger_DestroyHotarus(void);

#endif //! OBJ_SSZ1OUTRO_H
