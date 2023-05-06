#ifndef OBJ_MMZLIGHTNING_H
#define OBJ_MMZLIGHTNING_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectMMZLightning {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityMMZLightning {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 unused1;
    int32 unused2;
    Animator animator;
};

// Object Struct
extern ObjectMMZLightning *MMZLightning;

// Standard Entity Events
void MMZLightning_Update(void);
void MMZLightning_LateUpdate(void);
void MMZLightning_StaticUpdate(void);
void MMZLightning_Draw(void);
void MMZLightning_Create(void *data);
void MMZLightning_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MMZLightning_EditorDraw(void);
void MMZLightning_EditorLoad(void);
#endif
void MMZLightning_Serialize(void);

// Extra Entity Functions
void MMZLightning_State_BeginFadeIn(void);
void MMZLightning_State_FadeIn(void);
void MMZLightning_State_SetupLightningBig(void);
void MMZLightning_State_ShowLightningBig(void);
void MMZLightning_State_LightningBigFadeOut(void);
void MMZLightning_State_SetupLightningSmall(void);
void MMZLightning_State_LightningSmallFadeOut(void);

#endif

#endif //! OBJ_MMZLIGHTNING_H
