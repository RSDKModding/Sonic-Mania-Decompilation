#ifndef OBJ_JUGGLESAW_H
#define OBJ_JUGGLESAW_H

#include "Game.h"

typedef enum { JUGGLESAW_NO_SAW = 0, JUGGLESAW_HAS_SAW = 1, JUGGLESAW_AWAITING_SAW = 2 } JuggleSawMode;

#define JUGGLESAW_MAX_FRIENDS (8)

// Object Class
struct ObjectJuggleSaw {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxFriendRange;
    Hitbox hitboxSaw;
    Hitbox hitboxGrabV;
    Hitbox hitboxGrabH;
    uint16 aniFrames;
    uint16 sfxExplode;
    uint16 sfxJuggle;
    uint16 sfxThrow;
};

// Entity Class
struct EntityJuggleSaw {
    RSDK_ENTITY
    StateMachine(state);
    uint16 sawDelay;
    uint32 sawSpeed;
    bool32 hasSaw;
    uint16 setID;
    EntityJuggleSaw *friends[JUGGLESAW_MAX_FRIENDS];
    uint8 friendCount;
    uint8 sawTimer;
    Vector2 startPos;
    uint8 startDir;
    Animator animator;
};

// Object Struct
extern ObjectJuggleSaw *JuggleSaw;

// Standard Entity Events
void JuggleSaw_Update(void);
void JuggleSaw_LateUpdate(void);
void JuggleSaw_StaticUpdate(void);
void JuggleSaw_Draw(void);
void JuggleSaw_Create(void *data);
void JuggleSaw_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void JuggleSaw_EditorDraw(void);
void JuggleSaw_EditorLoad(void);
#endif
void JuggleSaw_Serialize(void);

void JuggleSaw_DebugDraw(void);
void JuggleSaw_DebugSpawn(void);

// Extra Entity Functions
void JuggleSaw_CheckPlayerCollisions(void);
void JuggleSaw_CheckOffScreen(void);

void JuggleSaw_StateCrab_Setup(void);
void JuggleSaw_StateCrab_Handle(void);
void JuggleSaw_StateCrab_ThrowSaw(void);

void JuggleSaw_StateSaw_Handle(void);
void JuggleSaw_StateSaw_Debris(void);

#endif //! OBJ_JUGGLESAW_H
