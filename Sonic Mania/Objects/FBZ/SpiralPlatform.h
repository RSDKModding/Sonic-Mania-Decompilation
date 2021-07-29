#ifndef OBJ_SPIRALPLATFORM_H
#define OBJ_SPIRALPLATFORM_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    ushort field_1C;
    Hitbox hitbox;
} ObjectSpiralPlatform;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    int type;
    Vector2 offRange;
    int speed;
    bool32 hasTension;
    sbyte frameID;
    byte collision;
    Vector2 targetPos;
    Vector2 centerPos;
    Vector2 drawPos;
    Vector2 collisionOffset;
    int stood;
    int collapseDelay;
    int stoodAngle;
    byte stoodPlayers;
    byte pushPlayersL;
    byte pushPlayersR;
    Hitbox hitbox;
    Animator animator;
    int childCount;
    Vector2 tileSize;
    ushort tiles[0x100];
    int fallY;
} EntitySpiralPlatform;

// Object Struct
extern ObjectSpiralPlatform *SpiralPlatform;

// Standard Entity Events
void SpiralPlatform_Update(void);
void SpiralPlatform_LateUpdate(void);
void SpiralPlatform_StaticUpdate(void);
void SpiralPlatform_Draw(void);
void SpiralPlatform_Create(void *data);
void SpiralPlatform_StageLoad(void);
void SpiralPlatform_EditorDraw(void);
void SpiralPlatform_EditorLoad(void);
void SpiralPlatform_Serialize(void);

// Extra Entity Functions
void SpiralPlatform_Unknown1(void);

#endif //! OBJ_SPIRALPLATFORM_H
