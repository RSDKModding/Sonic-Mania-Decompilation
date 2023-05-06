#ifndef OBJ_GONDOLA_H
#define OBJ_GONDOLA_H

#include "Game.h"

// Object Class
struct ObjectGondola {
    RSDK_OBJECT
    uint16 aniFrames;
    bool32 taggedBoatIDs[3];
    bool32 hasAchievement;
};

// Entity Class
struct EntityGondola {
    RSDK_ENTITY
    StateMachine(state);
    int32 minY;
    int32 maxY;
    uint8 boatID;
    uint8 activePlayers;
    Vector2 startPos;
    int32 unused;
    bool32 waterInRange;
    bool32 stoppedL;
    bool32 stoppedR;
    int32 waterDistance;
    Vector2 centerPos;
    Vector2 drawPos;
    Vector2 collisionOffset;
    int32 stoodAngle;
    int32 maxStoodAngle;
    int32 floatAngle;
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectGondola *Gondola;

// Standard Entity Events
void Gondola_Update(void);
void Gondola_LateUpdate(void);
void Gondola_StaticUpdate(void);
void Gondola_Draw(void);
void Gondola_Create(void *data);
void Gondola_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Gondola_EditorDraw(void);
void Gondola_EditorLoad(void);
#endif
void Gondola_Serialize(void);

// Extra Entity Functions
int32 Gondola_GetWaterLevel(void);
void Gondola_HandleWaterFloating(void);
void Gondola_HandleTilting(void);
void Gondola_HandleMoveVelocity(void);
void Gondola_HandleTileCollisions(void);
void Gondola_HandlePlayerInteractions(void);

#endif //! OBJ_GONDOLA_H
