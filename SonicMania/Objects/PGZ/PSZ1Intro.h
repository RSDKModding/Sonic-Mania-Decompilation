#ifndef OBJ_PSZ1INTRO_H
#define OBJ_PSZ1INTRO_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectPSZ1Intro {
    RSDK_OBJECT
};

// Entity Class
struct EntityPSZ1Intro {
    RSDK_ENTITY
    int32 timer;
    EntityHangGlider *gliders[PLAYER_COUNT];
};

// Object Struct
extern ObjectPSZ1Intro *PSZ1Intro;

// Standard Entity Events
void PSZ1Intro_Update(void);
void PSZ1Intro_LateUpdate(void);
void PSZ1Intro_StaticUpdate(void);
void PSZ1Intro_Draw(void);
void PSZ1Intro_Create(void *data);
void PSZ1Intro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PSZ1Intro_EditorDraw(void);
void PSZ1Intro_EditorLoad(void);
#endif
void PSZ1Intro_Serialize(void);

// Extra Entity Functions
void PSZ1Intro_HandleGliderJump(EntityHangGlider *glider);

bool32 PSZ1Intro_Cutscene_SetupGliders(EntityCutsceneSeq *host);
bool32 PSZ1Intro_Cutscene_GlideAndJump(EntityCutsceneSeq *host);
bool32 PSZ1Intro_Cutscene_HandleLanding(EntityCutsceneSeq *host);

#endif

#endif //! OBJ_PSZ1INTRO_H
