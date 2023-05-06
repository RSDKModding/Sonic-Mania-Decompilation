#ifndef OBJ_FORCESPIN_H
#define OBJ_FORCESPIN_H

#include "Game.h"

// Object Class
struct ObjectForceSpin {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityForceSpin {
    RSDK_ENTITY
    int32 size;
    int32 negAngle;
    Animator animator;
};

// Object Struct
extern ObjectForceSpin *ForceSpin;

// Standard Entity Events
void ForceSpin_Update(void);
void ForceSpin_LateUpdate(void);
void ForceSpin_StaticUpdate(void);
void ForceSpin_Draw(void);
void ForceSpin_Create(void *data);
void ForceSpin_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ForceSpin_EditorDraw(void);
void ForceSpin_EditorLoad(void);
#endif
void ForceSpin_Serialize(void);

// Extra Entity Functions
void ForceSpin_DrawSprites(void);
void ForceSpin_SetPlayerState(EntityPlayer *player);

#endif //! OBJ_FORCESPIN_H
