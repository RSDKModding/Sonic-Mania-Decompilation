#ifndef OBJ_SPIRALPLATFORM_H
#define OBJ_SPIRALPLATFORM_H

#include "Game.h"

// Object Class
struct ObjectSpiralPlatform {
    RSDK_OBJECT
    Animator animator;
    uint16 aniFrames;
    Hitbox hitboxPlatform;
};

// Entity Class
struct EntitySpiralPlatform {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    int32 type;
    Vector2 offRange;
    int32 speed;
    bool32 hasTension;
    int8 frameID;
    uint8 collision;
    Vector2 targetPos;
    Vector2 centerPos;
    Vector2 drawPos;
    Vector2 collisionOffset;
    int32 stood;
    int32 timer;
    int32 stoodAngle;
    uint8 stoodPlayers;
    uint8 pushPlayersL;
    uint8 pushPlayersR;
    Hitbox hitbox;
    Animator animator;
    int32 childCount;
    Vector2 tileSize;
    uint16 tiles[0x100];

    int32 fallY;
};

// Object Struct
extern ObjectSpiralPlatform *SpiralPlatform;

// Standard Entity Events
void SpiralPlatform_Update(void);
void SpiralPlatform_LateUpdate(void);
void SpiralPlatform_StaticUpdate(void);
void SpiralPlatform_Draw(void);
void SpiralPlatform_Create(void *data);
void SpiralPlatform_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SpiralPlatform_EditorDraw(void);
void SpiralPlatform_EditorLoad(void);
#endif
void SpiralPlatform_Serialize(void);

// Extra Entity Functions
void SpiralPlatform_State_Move(void);

#endif //! OBJ_SPIRALPLATFORM_H
