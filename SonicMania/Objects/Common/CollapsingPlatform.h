#ifndef OBJ_COLLAPSINGPLATFORM_H
#define OBJ_COLLAPSINGPLATFORM_H

#include "Game.h"

typedef enum {
    COLLAPSEPLAT_LEFT,
    COLLAPSEPLAT_RIGHT,
    COLLAPSEPLAT_CENTER,
    COLLAPSEPLAT_LR,
    COLLAPSEPLAT_LRC,
} CollapsingPlatformTypes;

typedef enum {
    COLLAPSEPLAT_TARGET_LOW,
    COLLAPSEPLAT_TARGET_HIGH,
} CollapsingPlatformTargetLayers;

// Object Class
struct ObjectCollapsingPlatform {
    RSDK_OBJECT
    uint8 shift;
    Animator animator;
    uint16 aniFrames;
    uint16 sfxCrumble;
};

// Entity Class
struct EntityCollapsingPlatform {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 size;
    bool32 respawn;
    uint16 targetLayer;
    uint8 type;
    int32 delay;
    bool32 eventOnly;
    bool32 mightyOnly;
    int32 unused1;
    int32 collapseDelay;
    uint16 storedTiles[256];
    Hitbox hitboxTrigger;
    Vector2 stoodPos;
};

// Object Struct
extern ObjectCollapsingPlatform *CollapsingPlatform;

// Standard Entity Events
void CollapsingPlatform_Update(void);
void CollapsingPlatform_LateUpdate(void);
void CollapsingPlatform_StaticUpdate(void);
void CollapsingPlatform_Draw(void);
void CollapsingPlatform_Create(void *data);
void CollapsingPlatform_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CollapsingPlatform_EditorDraw(void);
void CollapsingPlatform_EditorLoad(void);
#endif
void CollapsingPlatform_Serialize(void);

// Extra Entity Functions
void CollapsingPlatform_State_Left(void);
void CollapsingPlatform_State_Right(void);
void CollapsingPlatform_State_Center(void);
void CollapsingPlatform_State_LeftRight(void);
void CollapsingPlatform_State_LeftRightCenter(void);

#endif //! OBJ_COLLAPSINGPLATFORM_H
