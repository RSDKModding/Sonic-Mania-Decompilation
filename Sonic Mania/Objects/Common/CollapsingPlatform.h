#ifndef OBJ_COLLAPSINGPLATFORM_H
#define OBJ_COLLAPSINGPLATFORM_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint8 shift;
    Animator animator;
    uint16 aniFrames;
    uint16 sfxCrumble;
} ObjectCollapsingPlatform;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 size;
    bool32 respawn;
    uint16 targetLayer;
    uint8 type;
    int32 delay;
    bool32 eventOnly;
    bool32 mightyOnly;
    int32 field_78;
    int32 collapseDelay;
    uint16 storedTiles[256];
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
#if RETRO_INCLUDE_EDITOR
void CollapsingPlatform_EditorDraw(void);
void CollapsingPlatform_EditorLoad(void);
#endif
void CollapsingPlatform_Serialize(void);

// Extra Entity Functions
void CollapsingPlatform_State_Left(void);
void CollapsingPlatform_State_Right(void);
void CollapsingPlatform_State_Center(void);
void CollapsingPlatform_State_LeftOrRight(void);
void CollapsingPlatform_State_PlayerPos(void);

#endif //!OBJ_COLLAPSINGPLATFORM_H
