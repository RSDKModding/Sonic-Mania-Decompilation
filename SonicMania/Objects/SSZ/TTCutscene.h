#ifndef OBJ_TTCUTSCENE_H
#define OBJ_TTCUTSCENE_H

#include "Game.h"

// Object Class
struct ObjectTTCutscene {
    RSDK_OBJECT
    int32 unused;
    EntityFXFade *fxFade;
};

// Entity Class
struct EntityTTCutscene {
    MANIA_CUTSCENE_BASE
};

// Object Struct
extern ObjectTTCutscene *TTCutscene;

// Standard Entity Events
void TTCutscene_Update(void);
void TTCutscene_LateUpdate(void);
void TTCutscene_StaticUpdate(void);
void TTCutscene_Draw(void);
void TTCutscene_Create(void *data);
void TTCutscene_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TTCutscene_EditorDraw(void);
void TTCutscene_EditorLoad(void);
#endif
void TTCutscene_Serialize(void);

// Extra Entity Functions
void TTCutscene_StartCutscene(void);
#if MANIA_USE_PLUS
void TTCutscene_Cutscene_SkipCB(void);
#endif

bool32 TTCutscene_Cutscene_Setup(EntityCutsceneSeq *host);
bool32 TTCutscene_Cutscene_FlyIn(EntityCutsceneSeq *host);
bool32 TTCutscene_Cutscene_Wait(EntityCutsceneSeq *host);
bool32 TTCutscene_Cutscene_FlyOut(EntityCutsceneSeq *host);
bool32 TTCutscene_Cutscene_NextScene(EntityCutsceneSeq *host);

#endif //! OBJ_TTCUTSCENE_H
