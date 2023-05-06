#ifndef OBJ_FIREFLIES_H
#define OBJ_FIREFLIES_H

#include "Game.h"

// Object Class
struct ObjectFireflies {
    RSDK_OBJECT
    uint16 aniFrames;
    int32 activeFireflyCount;
};

// Entity Class
struct EntityFireflies {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused1;
    int32 timer;
    int32 unused2;
    int32 unused3;
    Animator animator;
    int32 duration;
    int32 screenCount;
    int32 screenID;
    Vector2 points[4];
};

// Object Struct
extern ObjectFireflies *Fireflies;

// Standard Entity Events
void Fireflies_Update(void);
void Fireflies_LateUpdate(void);
void Fireflies_StaticUpdate(void);
void Fireflies_Draw(void);
void Fireflies_Create(void *data);
void Fireflies_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Fireflies_EditorDraw(void);
void Fireflies_EditorLoad(void);
#endif
void Fireflies_Serialize(void);

// Extra Entity Functions
void Fireflies_State_Spawner(void);
void Fireflies_State_Firefly(void);

#endif //! OBJ_FIREFLIES_H
