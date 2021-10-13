#ifndef OBJ_FORCESPIN_H
#define OBJ_FORCESPIN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectForceSpin;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int size;
    int negAngle;
    Animator animator;
} EntityForceSpin;

// Object Struct
extern ObjectForceSpin *ForceSpin;

// Standard Entity Events
void ForceSpin_Update(void);
void ForceSpin_LateUpdate(void);
void ForceSpin_StaticUpdate(void);
void ForceSpin_Draw(void);
void ForceSpin_Create(void* data);
void ForceSpin_StageLoad(void);
void ForceSpin_EditorDraw(void);
void ForceSpin_EditorLoad(void);
void ForceSpin_Serialize(void);

// Extra Entity Functions
void ForceSpin_DrawSprites(void);
void ForceSpin_SetPlayerState(void *plr);

#endif //!OBJ_FORCESPIN_H
