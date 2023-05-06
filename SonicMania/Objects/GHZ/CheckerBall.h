#ifndef OBJ_CHECKERBALL_H
#define OBJ_CHECKERBALL_H

#include "Game.h"

// Object Class
struct ObjectCheckerBall {
    RSDK_OBJECT
    Hitbox hitboxBall;
    Hitbox innerBox;
    Hitbox outerBox;
    uint8 heightArray[24];
    uint8 unusedArray[24];
    uint16 aniFrames;
};

// Entity Class
struct EntityCheckerBall {
    RSDK_ENTITY
    bool32 playerControlled;
    int32 angleVel;
    int32 ballAngle;
    Vector2 roundedPos;
    Vector2 collisionOffset;
    uint8 activePlayers;
    Animator animator;
};

// Object Struct
extern ObjectCheckerBall *CheckerBall;

// Standard Entity Events
void CheckerBall_Update(void);
void CheckerBall_LateUpdate(void);
void CheckerBall_StaticUpdate(void);
void CheckerBall_Draw(void);
void CheckerBall_Create(void *data);
void CheckerBall_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CheckerBall_EditorDraw(void);
void CheckerBall_EditorLoad(void);
#endif
void CheckerBall_Serialize(void);

// Extra Entity Functions
void CheckerBall_DebugSpawn(void);
void CheckerBall_DebugDraw(void);

void CheckerBall_HandlePhysics(void);
void CheckerBall_HandlePlayerMovement(void);
void CheckerBall_HandlePlayerInteractions(void);
void CheckerBall_BadnikBreak(void *b, Hitbox *hitbox);
void CheckerBall_HandleObjectCollisions(void);

#endif //! OBJ_CHECKERBALL_H
