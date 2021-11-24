#ifndef OBJ_HANGGLIDER_H
#define OBJ_HANGGLIDER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
} ObjectHangGlider;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused1;
    int32 gravityStrength;
    int32 unused2;
    int32 unused3;
    Entity *playerPtr;
    Animator sailAnimator;
    Animator handleBackAnimator;
    Animator handleFrontAnimator;
    Animator playerAnimator;
} EntityHangGlider;

// Object Struct
extern ObjectHangGlider *HangGlider;

// Standard Entity Events
void HangGlider_Update(void);
void HangGlider_LateUpdate(void);
void HangGlider_StaticUpdate(void);
void HangGlider_Draw(void);
void HangGlider_Create(void* data);
void HangGlider_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void HangGlider_EditorDraw(void);
void HangGlider_EditorLoad(void);
#endif
void HangGlider_Serialize(void);

// Extra Entity Functions
void HangGlider_Unknown1(void);
void HangGlider_Unknown2(void);


#endif //!OBJ_HANGGLIDER_H
