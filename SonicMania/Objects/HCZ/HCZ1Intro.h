#ifndef OBJ_HCZ1INTRO_H
#define OBJ_HCZ1INTRO_H

#include "Game.h"

// Object Class
struct ObjectHCZ1Intro {
    RSDK_OBJECT
};

// Entity Class
struct EntityHCZ1Intro {
    MANIA_CUTSCENE_BASE
};

// Object Struct
extern ObjectHCZ1Intro *HCZ1Intro;

// Standard Entity Events
void HCZ1Intro_Update(void);
void HCZ1Intro_LateUpdate(void);
void HCZ1Intro_StaticUpdate(void);
void HCZ1Intro_Draw(void);
void HCZ1Intro_Create(void *data);
void HCZ1Intro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HCZ1Intro_EditorDraw(void);
void HCZ1Intro_EditorLoad(void);
#endif
void HCZ1Intro_Serialize(void);

// Extra Entity Functions
bool32 HCZ1Intro_Cutscene_Intro(EntityCutsceneSeq *host);

#endif //! OBJ_HCZ1INTRO_H
