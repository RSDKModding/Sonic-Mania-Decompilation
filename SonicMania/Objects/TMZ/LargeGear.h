#ifndef OBJ_LARGEGEAR_H
#define OBJ_LARGEGEAR_H

#include "Game.h"

// Object Class
struct ObjectLargeGear {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxTooth;
    Hitbox hitboxBase;
};

// Entity Class
struct EntityLargeGear {
    RSDK_ENTITY
    int32 rotSpeed;
    int32 rotOffset;
    uint8 activePlayers;
    int32 playerIDs[PLAYER_COUNT];
    Vector2 positions[8];
    Vector2 origin;
    int32 unused;
    Animator baseAnimator;
    Animator toothAnimator;
    Animator axleAnimator;
    Animator centerAnimator;
    Animator rivetAnimator;
};

// Object Struct
extern ObjectLargeGear *LargeGear;

// Standard Entity Events
void LargeGear_Update(void);
void LargeGear_LateUpdate(void);
void LargeGear_StaticUpdate(void);
void LargeGear_Draw(void);
void LargeGear_Create(void *data);
void LargeGear_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LargeGear_EditorDraw(void);
void LargeGear_EditorLoad(void);
#endif
void LargeGear_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_LARGEGEAR_H
