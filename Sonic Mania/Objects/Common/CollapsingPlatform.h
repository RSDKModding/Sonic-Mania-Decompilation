#ifndef OBJ_COLLAPSINGPLATFORM_H
#define OBJ_COLLAPSINGPLATFORM_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    byte shift;
    Animator animator;
    ushort spriteIndex;
    ushort sfx_Crumble;
} ObjectCollapsingPlatform;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 size;
    bool32 respawn;
    ushort targetLayer;
    byte type;
    int delay;
    bool32 eventOnly;
    bool32 mightyOnly;
    int field_78;
    int collapseDelay;
    ushort storedTiles[256];
    Hitbox hitbox;
    Vector2 playerPos;
} EntityCollapsingPlatform;

// Object Struct
extern ObjectCollapsingPlatform *CollapsingPlatform;

// Standard Entity Events
void CollapsingPlatform_Update(void);
void CollapsingPlatform_LateUpdate(void);
void CollapsingPlatform_StaticUpdate(void);
void CollapsingPlatform_Draw(void);
void CollapsingPlatform_Create(void* data);
void CollapsingPlatform_StageLoad(void);
void CollapsingPlatform_EditorDraw(void);
void CollapsingPlatform_EditorLoad(void);
void CollapsingPlatform_Serialize(void);

// Extra Entity Functions
void CollapsingPlatform_State_Left(void);
void CollapsingPlatform_State_Right(void);
void CollapsingPlatform_State_Center(void);
void CollapsingPlatform_State_LeftOrRight(void);
void CollapsingPlatform_State_PlayerPos(void);

#endif //!OBJ_COLLAPSINGPLATFORM_H
