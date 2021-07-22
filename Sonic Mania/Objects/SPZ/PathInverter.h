#ifndef OBJ_PATHINVERTER_H
#define OBJ_PATHINVERTER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
} ObjectPathInverter;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    byte type;
    Vector2 size2;
    Vector2 size;
    Entity *playerPtrs[PLAYER_MAX];
    int groundVelStore[PLAYER_MAX];
    int playerFlipFlags[PLAYER_MAX];
    Animator animator;
} EntityPathInverter;

// Object Struct
extern ObjectPathInverter *PathInverter;

// Standard Entity Events
void PathInverter_Update(void);
void PathInverter_LateUpdate(void);
void PathInverter_StaticUpdate(void);
void PathInverter_Draw(void);
void PathInverter_Create(void* data);
void PathInverter_StageLoad(void);
void PathInverter_EditorDraw(void);
void PathInverter_EditorLoad(void);
void PathInverter_Serialize(void);

// Extra Entity Functions
void PathInverter_Unknown1(Entity *player);
void PathInverter_State_Horizontal(void);
void PathInverter_State_Vertical(void);

#endif //!OBJ_PATHINVERTER_H
