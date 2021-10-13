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
    int32 field_60;
    int32 field_64;
    Vector2 unknownPos1;
    int32 field_70;
    int32 field_74;
    Vector2 unknownPos2;
    Vector2 emeraldVelocity;
    Animator animator1;
    Animator animator2;
    Animator animator3;
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
void TryAgain_EditorDraw(void);
void TryAgain_EditorLoad(void);
void TryAgain_Serialize(void);

// Extra Entity Functions
void TryAgain_Unknown1(void);
void TryAgain_Unknown2(void);
void TryAgain_Unknown3(void);
void TryAgain_Unknown4(void);

#endif //!OBJ_TRYAGAIN_H
