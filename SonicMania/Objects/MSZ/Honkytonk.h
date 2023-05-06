#ifndef OBJ_HONKYTONK_H
#define OBJ_HONKYTONK_H

#include "Game.h"

// Object Class
struct ObjectHonkytonk {
    RSDK_OBJECT
    Hitbox hitboxTrigger;
    Hitbox hitboxRebound;
    uint16 sfxPiano;
    uint16 aniFrames;
};

// Entity Class
struct EntityHonkytonk {
    RSDK_ENTITY
    StateMachine(state);
    int32 negAngle;
    int32 depression;
    Animator animator;
};

// Object Struct
extern ObjectHonkytonk *Honkytonk;

// Standard Entity Events
void Honkytonk_Update(void);
void Honkytonk_LateUpdate(void);
void Honkytonk_StaticUpdate(void);
void Honkytonk_Draw(void);
void Honkytonk_Create(void *data);
void Honkytonk_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Honkytonk_EditorDraw(void);
void Honkytonk_EditorLoad(void);
#endif
void Honkytonk_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_HONKYTONK_H
