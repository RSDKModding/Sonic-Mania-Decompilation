#ifndef OBJ_PATHINVERTER_H
#define OBJ_PATHINVERTER_H

#include "SonicMania.h"

typedef enum {
    PATHINVERTER_H,
    PATHINVERTER_V,
} PathInverterTypes;

// Object Class
struct ObjectPathInverter {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityPathInverter {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    Vector2 size2;
    Vector2 size;
    Entity *playerPtrs[PLAYER_MAX];
    int32 groundVelStore[PLAYER_MAX];
    int32 playerFlipFlags[PLAYER_MAX];
    Animator animator;
};

// Object Struct
extern ObjectPathInverter *PathInverter;

// Standard Entity Events
void PathInverter_Update(void);
void PathInverter_LateUpdate(void);
void PathInverter_StaticUpdate(void);
void PathInverter_Draw(void);
void PathInverter_Create(void* data);
void PathInverter_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PathInverter_EditorDraw(void);
void PathInverter_EditorLoad(void);
#endif
void PathInverter_Serialize(void);

// Extra Entity Functions
void PathInverter_HandlePathSwitch(void *p);
void PathInverter_State_Horizontal(void);
void PathInverter_State_Vertical(void);

#endif //!OBJ_PATHINVERTER_H
