#ifndef OBJ_LARGEGEAR_H
#define OBJ_LARGEGEAR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
} ObjectLargeGear;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 rotSpeed;
    int32 rotOffset;
    uint8 activePlayers;
    int32 playerIDs[4];
    Vector2 positions[8];
    Vector2 origin;
    int32 field_BC;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
} EntityLargeGear;

// Object Struct
extern ObjectLargeGear *LargeGear;

// Standard Entity Events
void LargeGear_Update(void);
void LargeGear_LateUpdate(void);
void LargeGear_StaticUpdate(void);
void LargeGear_Draw(void);
void LargeGear_Create(void* data);
void LargeGear_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LargeGear_EditorDraw(void);
void LargeGear_EditorLoad(void);
#endif
void LargeGear_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LARGEGEAR_H
