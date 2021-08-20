#ifndef OBJ_JUGGLESAW_H
#define OBJ_JUGGLESAW_H

#include "SonicMania.h"

typedef enum { JSAW_NO_SAW = 0, JSAW_HAS_SAW = 1, JSAW_AWAITING_SAW = 2 } JuggleSawMode;

#define JuggleSaw_MaxFriends (8)

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox friendbox;
    Hitbox sawHitbox;
    Hitbox grabboxFloor;
    Hitbox grabboxWall;
    ushort animID;
    ushort explodeSFX;
    ushort juggleSFX;
    ushort throwSFX;
} ObjectJuggleSaw;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    ushort sawDelay;
    uint sawSpeed;
    JuggleSawMode hasSaw;
    ushort setID;
    Entity *friends[JuggleSaw_MaxFriends];
    byte friendCount;
    byte sawTimer;
    Vector2 spawnPos;
    byte spawnDir;
    Animator animator;
} EntityJuggleSaw;

// Object Struct
extern ObjectJuggleSaw *JuggleSaw;

// Standard Entity Events
void JuggleSaw_Update(void);
void JuggleSaw_LateUpdate(void);
void JuggleSaw_StaticUpdate(void);
void JuggleSaw_Draw(void);
void JuggleSaw_Create(void* data);
void JuggleSaw_StageLoad(void);
void JuggleSaw_EditorDraw(void);
void JuggleSaw_EditorLoad(void);
void JuggleSaw_Serialize(void);

void JuggleSaw_DebugDraw(void);
void JuggleSaw_DebugSpawn(void);

// Extra Entity Functions
void JuggleSaw_Crab_Collide(void);
void JuggleSaw_Crab_CheckOnScreen(void);
void JuggleSaw_Crab_Handle(void);
void JuggleSaw_Crab_Create(void);
void JuggleSaw_Crab_ThrowSaw(void);

void JuggleSaw_Saw_Handle(void);
void JuggleSaw_Saw_Knocked(void);

#endif //!OBJ_JUGGLESAW_H
