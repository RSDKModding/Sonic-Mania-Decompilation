#ifndef OBJ_MMZ2OUTRO_H
#define OBJ_MMZ2OUTRO_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectMMZ2Outro {
    RSDK_OBJECT
    uint16 sfxThunda;
    uint16 sfxLightsOut;
    uint16 sfxTMZEyes;
};

// Entity Class
struct EntityMMZ2Outro {
    RSDK_ENTITY
    int32 timer;
    int32 flashTimer;
};

// Object Struct
extern ObjectMMZ2Outro *MMZ2Outro;

// Standard Entity Events
void MMZ2Outro_Update(void);
void MMZ2Outro_LateUpdate(void);
void MMZ2Outro_StaticUpdate(void);
void MMZ2Outro_Draw(void);
void MMZ2Outro_Create(void *data);
void MMZ2Outro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MMZ2Outro_EditorDraw(void);
void MMZ2Outro_EditorLoad(void);
#endif
void MMZ2Outro_Serialize(void);

// Extra Entity Functions
void MMZ2Outro_StartCutscene(void);

bool32 MMZ2Outro_Cutscene_PowerDown(EntityCutsceneSeq *host);
bool32 MMZ2Outro_Cutscene_Rumble(EntityCutsceneSeq *host);
bool32 MMZ2Outro_Cutscene_CameraMoveToWindow(EntityCutsceneSeq *host);
bool32 MMZ2Outro_Cutscene_PlayerMoveToWindow(EntityCutsceneSeq *host);
bool32 MMZ2Outro_Cutscene_EnterMonarchEyes(EntityCutsceneSeq *host);
bool32 MMZ2Outro_Cutscene_ViewMonarch(EntityCutsceneSeq *host);
#endif

#endif //! OBJ_MMZ2OUTRO_H
