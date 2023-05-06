#ifndef OBJ_HANGGLIDER_H
#define OBJ_HANGGLIDER_H

#include "Game.h"

#if MANIA_USE_PLUS

// Object Class
struct ObjectHangGlider {
    RSDK_OBJECT
    Hitbox hitboxUnused;
    uint16 aniFrames;
};

// Entity Class
struct EntityHangGlider {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused1;
    int32 gravityStrength;
    int32 unused2;
    int32 unused3;
    EntityPlayer *attachedPlayer;
    Animator sailAnimator;
    Animator handleBackAnimator;
    Animator handleFrontAnimator;
    Animator playerAnimator;
};

// Object Struct
extern ObjectHangGlider *HangGlider;

// Standard Entity Events
void HangGlider_Update(void);
void HangGlider_LateUpdate(void);
void HangGlider_StaticUpdate(void);
void HangGlider_Draw(void);
void HangGlider_Create(void *data);
void HangGlider_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HangGlider_EditorDraw(void);
void HangGlider_EditorLoad(void);
#endif
void HangGlider_Serialize(void);

// Extra Entity Functions
void HangGlider_State_CheckGrab(void);
void HangGlider_State_Glide(void);

#endif

#endif //! OBJ_HANGGLIDER_H
