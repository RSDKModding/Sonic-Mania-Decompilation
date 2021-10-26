#ifndef OBJ_COLLAPSINGSAND_H
#define OBJ_COLLAPSINGSAND_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Animator animator;
    uint16 aniFrames;
    uint16 sfxSandFall;
} ObjectCollapsingSand;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 size;
    int32 delay;
    int32 collapseTimer;
    bool32 collapseLeft;
    int32 field_70;
    int32 field_74;
    int32 tileX;
    int32 tileY;
    int32 tileMaxX;
    int32 tileMaxY;
    Hitbox hitbox;
} EntityCollapsingSand;

// Object Struct
extern ObjectCollapsingSand *CollapsingSand;

// Standard Entity Events
void CollapsingSand_Update(void);
void CollapsingSand_LateUpdate(void);
void CollapsingSand_StaticUpdate(void);
void CollapsingSand_Draw(void);
void CollapsingSand_Create(void* data);
void CollapsingSand_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void CollapsingSand_EditorDraw(void);
void CollapsingSand_EditorLoad(void);
#endif
void CollapsingSand_Serialize(void);

// Extra Entity Functions
void CollapsingSand_State_CheckPlayerCollisions(void);
void CollapsingSand_State_CollapseDelay(void);
void CollapsingSand_State_CollapseLeft(void);
void CollapsingSand_State_CollapseRight(void);

#endif //!OBJ_COLLAPSINGSAND_H
