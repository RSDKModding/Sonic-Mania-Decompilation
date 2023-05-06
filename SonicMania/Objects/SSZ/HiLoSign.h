#ifndef OBJ_HILOSIGN_H
#define OBJ_HILOSIGN_H

#include "Game.h"

typedef enum {
    HILOSIGN_H,
    HILOSIGN_V,
} HiLoSignTypes;

// Object Class
struct ObjectHiLoSign {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox spinHitbox[2];
    uint16 sfxSignPost;
};

// Entity Class
struct EntityHiLoSign {
    RSDK_ENTITY
    StateMachine(stateDraw);
    StateMachine(state);
    uint8 type;
    int32 spinSpeed;
    Animator faceAnimator;
    Animator sidesAnimator;
};

// Object Struct
extern ObjectHiLoSign *HiLoSign;

// Standard Entity Events
void HiLoSign_Update(void);
void HiLoSign_LateUpdate(void);
void HiLoSign_StaticUpdate(void);
void HiLoSign_Draw(void);
void HiLoSign_Create(void *data);
void HiLoSign_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HiLoSign_EditorDraw(void);
void HiLoSign_EditorLoad(void);
#endif
void HiLoSign_Serialize(void);

// Extra Entity Functions
void HiLoSign_DebugSpawn(void);
void HiLoSign_DebugDraw(void);

void HiLoSign_State_Spinning(void);
void HiLoSign_State_FinishSpinAdjust(void);

void HiLoSign_Draw_Horizontal(void);
void HiLoSign_Draw_Vertical(void);

#endif //! OBJ_HILOSIGN_H
