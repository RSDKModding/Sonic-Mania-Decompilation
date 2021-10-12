#ifndef OBJ_LARGEGEAR_H
#define OBJ_LARGEGEAR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
} ObjectLargeGear;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int rotSpeed;
    int rotOffset;
    byte activePlayers;
    int playerIDs[4];
    Vector2 positions[8];
    Vector2 origin;
    int field_BC;
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
void LargeGear_EditorDraw(void);
void LargeGear_EditorLoad(void);
void LargeGear_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LARGEGEAR_H
