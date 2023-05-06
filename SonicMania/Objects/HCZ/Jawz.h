#ifndef OBJ_JAWZ_H
#define OBJ_JAWZ_H

#include "Game.h"

typedef enum {
    JAWS_TRIGGER_BOTH,
    JAWS_TRIGGER_LEFT,
    JAWS_TRIGGER_RIGHT,
} JawzTriggerDirs;

// Object Class
struct ObjectJawz {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    uint16 aniFrames;
};

// Entity Class
struct EntityJawz {
    RSDK_ENTITY
    uint8 triggerDir;
    StateMachine(state);
    Vector2 startPos;
    Animator animator;
};

// Object Struct
extern ObjectJawz *Jawz;

// Standard Entity Events
void Jawz_Update(void);
void Jawz_LateUpdate(void);
void Jawz_StaticUpdate(void);
void Jawz_Draw(void);
void Jawz_Create(void *data);
void Jawz_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Jawz_EditorDraw(void);
void Jawz_EditorLoad(void);
#endif
void Jawz_Serialize(void);

// Extra Entity Functions
void Jawz_DebugSpawn(void);
void Jawz_DebugDraw(void);
void Jawz_CheckPlayerCollisions(void);
void Jawz_State_CheckPlayerTrigger(void);
void Jawz_State_Triggered(void);

#endif //! OBJ_JAWZ_H
