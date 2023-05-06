#ifndef OBJ_INVINCIBLESTARS_H
#define OBJ_INVINCIBLESTARS_H

#include "Game.h"

// Object Class
struct ObjectInvincibleStars {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityInvincibleStars {
    RSDK_ENTITY
    EntityPlayer *player;
    Vector2 starPos[8];
    int32 starAngle[2];
    int32 starFrame[2];
    int32 starOffset;
    Animator starAnimator[4];
};

// Object Struct
extern ObjectInvincibleStars *InvincibleStars;

// Standard Entity Events
void InvincibleStars_Update(void);
void InvincibleStars_LateUpdate(void);
void InvincibleStars_StaticUpdate(void);
void InvincibleStars_Draw(void);
void InvincibleStars_Create(void *data);
void InvincibleStars_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void InvincibleStars_EditorDraw(void);
void InvincibleStars_EditorLoad(void);
#endif
void InvincibleStars_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_INVINCIBLESTARS_H
