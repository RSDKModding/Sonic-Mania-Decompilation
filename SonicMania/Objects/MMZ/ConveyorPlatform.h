#ifndef OBJ_CONVEYORPLATFORM_H
#define OBJ_CONVEYORPLATFORM_H

#include "Game.h"

typedef enum {
    CONVEYORPLATFORM_FIXED,
    CONVEYORPLATFORM_MOVING,
} ConveyorPlatformTypes;

// Object Class
struct ObjectConveyorPlatform {
    RSDK_OBJECT
    TABLE(int32 frameIDs[24], { 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1 });
    TABLE(int32 directionIDs[24], { 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1 });
};

// Entity Class
struct EntityConveyorPlatform {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    int32 type;
    Vector2 amplitude;
    int32 node;
    bool32 hasTension;
    int8 frameID;
    uint8 collision;
    Vector2 tileOrigin;
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

    uint16 interval;
    uint16 intervalOffset;
    uint16 flipCount;
    uint8 flipTimer;
};

// Object Struct
extern ObjectConveyorPlatform *ConveyorPlatform;

// Standard Entity Events
void ConveyorPlatform_Update(void);
void ConveyorPlatform_LateUpdate(void);
void ConveyorPlatform_StaticUpdate(void);
void ConveyorPlatform_Draw(void);
void ConveyorPlatform_Create(void *data);
void ConveyorPlatform_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ConveyorPlatform_EditorDraw(void);
void ConveyorPlatform_EditorLoad(void);
#endif
void ConveyorPlatform_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_CONVEYORPLATFORM_H
