#ifndef OBJ_TRYAGAIN_H
#define OBJ_TRYAGAIN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectTryAgain;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 unused1;
    int32 unused2;
    Vector2 eggmanPos;
    int32 unused3;
    int32 eggmanVelocityY;
    Vector2 rubyPos;
    Vector2 rubyVelocity;
    Animator debrisAnimator;
    Animator eggmanAnimator;
    Animator rubyAnimator;
} EntityTryAgain;

// Object Struct
extern ObjectTryAgain *TryAgain;

// Standard Entity Events
void TryAgain_Update(void);
void TryAgain_LateUpdate(void);
void TryAgain_StaticUpdate(void);
void TryAgain_Draw(void);
void TryAgain_Create(void* data);
void TryAgain_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void TryAgain_EditorDraw(void);
void TryAgain_EditorLoad(void);
#endif
void TryAgain_Serialize(void);

// Extra Entity Functions
void TryAgain_Unknown1(void);
void TryAgain_Unknown2(void);
void TryAgain_Unknown3(void);
void TryAgain_Unknown4(void);

#endif //!OBJ_TRYAGAIN_H
