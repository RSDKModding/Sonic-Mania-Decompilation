#ifndef OBJ_SPINSIGN_H
#define OBJ_SPINSIGN_H

#include "Game.h"

typedef enum {
    SPINSIGN_SONIC_H,
    SPINSIGN_SONIC_V,
    SPINSIGN_MANIA_H,
    SPINSIGN_MANIA_V,
} SpinSignTypes;

// Object Class
struct ObjectSpinSign {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxes[4];
    uint16 sfxSignPost;
};

// Entity Class
struct EntitySpinSign {
    RSDK_ENTITY
    StateMachine(stateDraw);
    StateMachine(state);
    uint8 type;
    int32 timer;
    Animator eggmanAnimator;
    Animator frontAnimator;
    Animator sidesAnimator;
};

// Object Struct
extern ObjectSpinSign *SpinSign;

// Standard Entity Events
void SpinSign_Update(void);
void SpinSign_LateUpdate(void);
void SpinSign_StaticUpdate(void);
void SpinSign_Draw(void);
void SpinSign_Create(void *data);
void SpinSign_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SpinSign_EditorDraw(void);
void SpinSign_EditorLoad(void);
#endif
void SpinSign_Serialize(void);

// Extra Entity Functions
void SpinSign_DebugSpawn(void);
void SpinSign_DebugDraw(void);

void SpinSign_State_Spinning(void);
void SpinSign_State_SlowDown(void);

void SpinSign_Draw_SonicH(void);
void SpinSign_Draw_SonicV(void);
void SpinSign_Draw_ManiaH(void);
void SpinSign_Draw_ManiaV(void);

#endif //! OBJ_SPINSIGN_H
