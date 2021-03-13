#ifndef OBJ_STARPOST_H
#define OBJ_STARPOST_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    bool32 hasAchievement;
    Vector2 playerPositions[4];
    byte playerDirections[4];
    ushort postIDs[4];
    byte storedMinutes;
    byte storedSeconds;
    byte storedMS;
    byte activePlayers;
    ushort spriteIndex;
    ushort sfx_StarPost;
    ushort sfx_Warp;
} ObjectStarPost;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    int id;
    bool32 vsRemove;
    int ballSpeed;
    int timer;
    int timer2;
    int starTimer;
    int starAngle2;
    int starAngle;
    int starOffset;
    Vector2 ballPos;
    EntityAnimationData poleData;
    EntityAnimationData ballData;
    EntityAnimationData starData;
    Hitbox starHitbox;
    byte activated;
} EntityStarPost;

// Object Struct
extern ObjectStarPost *StarPost;

// Standard Entity Events
void StarPost_Update();
void StarPost_LateUpdate();
void StarPost_StaticUpdate();
void StarPost_Draw();
void StarPost_Create(void* data);
void StarPost_StageLoad();
void StarPost_EditorDraw();
void StarPost_EditorLoad();
void StarPost_Serialize();

// Extra Entity Functions
void StarPost_DebugDraw();
void StarPost_DebugSpawn();
void StarPost_ResetStarPosts();
void StarPost_CheckBonusStageEntry();
void StarPost_CheckCollisions();
void StarPost_State_Idle();
void StarPost_State_BallSpin();

#endif //!OBJ_STARPOST_H
