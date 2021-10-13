#ifndef OBJ_INVINCIBLESTARS_H
#define OBJ_INVINCIBLESTARS_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
} ObjectInvincibleStars;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Entity *parent;
    Vector2 starPos[8];
    int32 starAngle[4];
    int32 starOffset;
    Animator starsData[4];
} EntityInvincibleStars;

// Object Struct
extern ObjectInvincibleStars *InvincibleStars;

// Standard Entity Events
void InvincibleStars_Update(void);
void InvincibleStars_LateUpdate(void);
void InvincibleStars_StaticUpdate(void);
void InvincibleStars_Draw(void);
void InvincibleStars_Create(void* data);
void InvincibleStars_StageLoad(void);
void InvincibleStars_EditorDraw(void);
void InvincibleStars_EditorLoad(void);
void InvincibleStars_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_INVINCIBLESTARS_H
