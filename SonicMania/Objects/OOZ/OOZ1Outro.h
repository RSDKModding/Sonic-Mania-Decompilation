#ifndef OBJ_OOZ1OUTRO_H
#define OBJ_OOZ1OUTRO_H

#include "Game.h"

// Object Class
struct ObjectOOZ1Outro {
    RSDK_OBJECT
    int32 unused;
    uint16 sfxDrop;
};

// Entity Class
struct EntityOOZ1Outro {
    MANIA_CUTSCENE_BASE
    int32 boundsL;
    int32 boundsR;
    int32 boundsT;
    int32 boundsB;
};

// Object Struct
extern ObjectOOZ1Outro *OOZ1Outro;

// Standard Entity Events
void OOZ1Outro_Update(void);
void OOZ1Outro_LateUpdate(void);
void OOZ1Outro_StaticUpdate(void);
void OOZ1Outro_Draw(void);
void OOZ1Outro_Create(void *data);
void OOZ1Outro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void OOZ1Outro_EditorDraw(void);
void OOZ1Outro_EditorLoad(void);
#endif
void OOZ1Outro_Serialize(void);

// Extra Entity Functions
bool32 OOZ1Outro_Cutscene_FadeIn(EntityCutsceneSeq *host);
bool32 OOZ1Outro_Cutscene_PostActClearSetup(EntityCutsceneSeq *host);
bool32 OOZ1Outro_Cutscene_FallIntoAct2(EntityCutsceneSeq *host);
bool32 OOZ1Outro_Cutscene_BeginAct(EntityCutsceneSeq *host);

#endif //! OBJ_OOZ1OUTRO_H
