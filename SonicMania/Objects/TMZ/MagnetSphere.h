#ifndef OBJ_MAGNETSPHERE_H
#define OBJ_MAGNETSPHERE_H

#include "Game.h"

// Object Class
struct ObjectMagnetSphere {
    RSDK_OBJECT
    Hitbox hitbox;
    uint8 sphereAngle[PLAYER_COUNT];
    uint8 playerAngles[PLAYER_COUNT];
    TABLE(int32 angleTable[16], { 0x20, 0x20, 0x20, 0x30, 0x40, 0x50, 0x60, 0x60, 0x60, 0xA0, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xE0 });
    uint16 aniFrames;
    uint16 sfxBumper;
    uint16 sfxPlasmaBall;
};

// Entity Class
struct EntityMagnetSphere {
    RSDK_ENTITY
    uint8 activePlayers;
    uint8 playerTimers[PLAYER_COUNT];
    Animator unusedAnimator;
    Animator glassInnerAnimator;
    Animator glassOuterAnimator;
    Animator plasma1Animator;
    Animator plasma2Animator;
    Animator centerBallAnimator;
};

// Object Struct
extern ObjectMagnetSphere *MagnetSphere;

// Standard Entity Events
void MagnetSphere_Update(void);
void MagnetSphere_LateUpdate(void);
void MagnetSphere_StaticUpdate(void);
void MagnetSphere_Draw(void);
void MagnetSphere_Create(void *data);
void MagnetSphere_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MagnetSphere_EditorDraw(void);
void MagnetSphere_EditorLoad(void);
#endif
void MagnetSphere_Serialize(void);

// Extra Entity Functions
void MagnetSphere_DebugSpawn(void);
void MagnetSphere_DebugDraw(void);

void MagnetSphere_CheckPlayerCollision(void);
void MagnetSphere_MovePlayer(EntityPlayer *player, int32 playerID);

#endif //! OBJ_MAGNETSPHERE_H
