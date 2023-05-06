#ifndef OBJ_COLLAPSINGSAND_H
#define OBJ_COLLAPSINGSAND_H

#include "Game.h"

// Object Class
struct ObjectCollapsingSand {
    RSDK_OBJECT
    Animator animator;
    uint16 aniFrames;
    uint16 sfxSandFall;
};

// Entity Class
struct EntityCollapsingSand {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 size;
    int32 delay;
    int32 collapseTimer;
    bool32 collapseLeft;
    Vector2 collapseDuration;
    Vector2 tilePos;
    Vector2 tileEndPos;
    Hitbox hitbox;
};

// Object Struct
extern ObjectCollapsingSand *CollapsingSand;

// Standard Entity Events
void CollapsingSand_Update(void);
void CollapsingSand_LateUpdate(void);
void CollapsingSand_StaticUpdate(void);
void CollapsingSand_Draw(void);
void CollapsingSand_Create(void *data);
void CollapsingSand_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CollapsingSand_EditorDraw(void);
void CollapsingSand_EditorLoad(void);
#endif
void CollapsingSand_Serialize(void);

// Extra Entity Functions
void CollapsingSand_State_CheckPlayerCollisions(void);
void CollapsingSand_State_CollapseDelay(void);
void CollapsingSand_State_CollapseLeft(void);
void CollapsingSand_State_CollapseRight(void);

#endif //! OBJ_COLLAPSINGSAND_H
