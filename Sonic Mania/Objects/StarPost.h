#ifndef OBJ_STARPOST_H
#define OBJ_STARPOST_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    bool32 hasAchievement;
    Vector2 playerPositions[PLAYER_MAX];
    byte playerDirections[PLAYER_MAX];
    ushort postIDs[PLAYER_MAX];
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
    StateMachine(state);
    int id;
    bool32 vsRemove;
    int ballSpeed;
    int timer;
    int starTimer;
    int starFlag;
    int starAngle2;
    int starAngle;
    int starOffset;
    Vector2 ballPos;
    Animator poleData;
    Animator ballData;
    Animator starData;
    Hitbox starHitbox;
    byte activated;
} EntityStarPost;

// Object Struct
extern ObjectStarPost *StarPost;

// Standard Entity Events
void StarPost_Update(void);
void StarPost_LateUpdate(void);
void StarPost_StaticUpdate(void);
void StarPost_Draw(void);
void StarPost_Create(void* data);
void StarPost_StageLoad(void);
void StarPost_EditorDraw(void);
void StarPost_EditorLoad(void);
void StarPost_Serialize(void);

// Extra Entity Functions
void StarPost_DebugDraw(void);
void StarPost_DebugSpawn(void);
void StarPost_ResetStarPosts(void);
void StarPost_CheckBonusStageEntry(void);
void StarPost_CheckCollisions(void);
void StarPost_State_Idle(void);
void StarPost_State_BallSpin(void);

#endif //!OBJ_STARPOST_H
