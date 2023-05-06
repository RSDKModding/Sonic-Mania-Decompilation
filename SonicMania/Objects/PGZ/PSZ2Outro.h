#ifndef OBJ_PSZ2OUTRO_H
#define OBJ_PSZ2OUTRO_H

#include "Game.h"

// Object Class
struct ObjectPSZ2Outro {
    RSDK_OBJECT
    int32 unused;
};

// Entity Class
struct EntityPSZ2Outro {
    MANIA_CUTSCENE_BASE
    EntityPSZEggman *eggman;
    EntityPhantomRuby *ruby;
    EntityFXRuby *fxRuby;
};

// Object Struct
extern ObjectPSZ2Outro *PSZ2Outro;

// Standard Entity Events
void PSZ2Outro_Update(void);
void PSZ2Outro_LateUpdate(void);
void PSZ2Outro_StaticUpdate(void);
void PSZ2Outro_Draw(void);
void PSZ2Outro_Create(void *data);
void PSZ2Outro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PSZ2Outro_EditorDraw(void);
void PSZ2Outro_EditorLoad(void);
#endif
void PSZ2Outro_Serialize(void);

// Extra Entity Functions
bool32 PSZ2Outro_Cutscene_SetupCameraMove(EntityCutsceneSeq *host);
bool32 PSZ2Outro_Cutscene_HandleCameraMovement(EntityCutsceneSeq *host);
bool32 PSZ2Outro_Cutscene_WalkIntoPlace(EntityCutsceneSeq *host);
bool32 PSZ2Outro_Cutscene_EnterRuby(EntityCutsceneSeq *host);
bool32 PSZ2Outro_Cutscene_RubyActivated(EntityCutsceneSeq *host);
bool32 PSZ2Outro_Cutscene_RubyWarp(EntityCutsceneSeq *host);
bool32 PSZ2Outro_Cutscene_LoadSSZ1(EntityCutsceneSeq *host);

#endif //! OBJ_PSZ2OUTRO_H
