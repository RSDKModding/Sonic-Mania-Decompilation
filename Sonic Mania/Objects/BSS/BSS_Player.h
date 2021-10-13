#ifndef OBJ_BSS_PLAYER_H
#define OBJ_BSS_PLAYER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 jumpPressState;
    uint16 field_6;
    uint16 sonicSpriteIndex;
    uint16 tailsSpriteIndex;
    uint16 knuxSpriteIndex;
    uint16 mightySpriteIndex;
    uint16 raySpriteIndex;
    uint16 sfx_Jump;
} ObjectBSS_Player;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 gravityStrength;
    bool32 sideKick;
    StateMachine(stateInput);
    int32 controllerID;
    bool32 up;
    bool32 down;
    bool32 left;
    bool32 right;
    bool32 jumpPress;
    Animator playerAnimator;
    Animator tailAnimator;
    uint16 spriteIndex;
    uint16 field_AE;
#if RETRO_USE_TOUCH_CONTROLS
    int32 touchJump;
#endif
} EntityBSS_Player;

// Object Struct
extern ObjectBSS_Player *BSS_Player;

// Standard Entity Events
void BSS_Player_Update(void);
void BSS_Player_LateUpdate(void);
void BSS_Player_StaticUpdate(void);
void BSS_Player_Draw(void);
void BSS_Player_Create(void* data);
void BSS_Player_StageLoad(void);
void BSS_Player_EditorDraw(void);
void BSS_Player_EditorLoad(void);
void BSS_Player_Serialize(void);

// Extra Entity Functions
void BSS_Player_ProcessP1Input(void);
void BSS_Player_ProcessP2Input(void);

#endif //!OBJ_BSS_PLAYER_H
