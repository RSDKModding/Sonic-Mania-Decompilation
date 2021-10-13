#ifndef OBJ_MAGNETSPHERE_H
#define OBJ_MAGNETSPHERE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint8 sphereAngle[4];
    uint8 playerAngles[4];
    TABLE(int32 angleList[16], { 32, 32, 32, 48, 64, 80, 96, 96, 96, 160, 160, 176, 192, 208, 224, 224 });
    uint16 aniFrames;
    uint16 sfxBumper;
    uint16 sfxPlasmaBall;
} ObjectMagnetSphere;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 activePlayers;
    uint8 playerTimers[4];
    int32 field_60;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
} EntityMagnetSphere;

// Object Struct
extern ObjectMagnetSphere *MagnetSphere;

// Standard Entity Events
void MagnetSphere_Update(void);
void MagnetSphere_LateUpdate(void);
void MagnetSphere_StaticUpdate(void);
void MagnetSphere_Draw(void);
void MagnetSphere_Create(void* data);
void MagnetSphere_StageLoad(void);
void MagnetSphere_EditorDraw(void);
void MagnetSphere_EditorLoad(void);
void MagnetSphere_Serialize(void);

// Extra Entity Functions
void MagnetSphere_DebugSpawn(void);
void MagnetSphere_DebugDraw(void);

void MagnetSphere_CheckPlayerCollision(void);
void MagnetSphere_MovePlayer(void *p, int32 playerID);

#endif //!OBJ_MAGNETSPHERE_H
