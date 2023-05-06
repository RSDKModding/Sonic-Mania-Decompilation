#ifndef OBJ_WARPDOOR_H
#define OBJ_WARPDOOR_H

#include "Game.h"

typedef enum { WARPDOOR_EFFECT_NONE } WarpDoorEffects;

typedef enum { WARPDOOR_PLRSTATE_NONE, WARPDOOR_PLRSTATE_HURT } WarpDoorPlayerStates;

// Object Class
struct ObjectWarpDoor {
    RSDK_OBJECT
    EntityWarpDoor *tags[256];
    Hitbox boundaries[256];
    uint8 boundCount;
    uint8 lastBoundsID;
    uint8 boundsTimer;
    bool32 hasSetupTags;
    bool32 hasSetupStartBounds;
    uint16 aniFrames;
    uint16 sfxRubyAttackL[6];
    uint16 sfxRubyAttackR[6];
    uint16 sfxWarpDoor;
};

// Entity Class
struct EntityWarpDoor {
    RSDK_ENTITY
    int32 go;
    int32 width;
    int32 height;
    uint8 tag;
    int32 destinationTag;
    bool32 definesBounds;
    int32 xBoundaryPosL;
    int32 xBoundaryPosR;
    int32 yBoundaryPosT;
    int32 yBoundaryPosB;
    uint8 effect;
    bool32 warpToCenter;
    uint8 forcePlayerState;
    Animator animator;
    int32 hasWarped;
    int32 unused1;
    uint16 boundID;
    int32 fadeTimer;
    bool32 fadeOut;
    Hitbox hitbox;
};

// Object Struct
extern ObjectWarpDoor *WarpDoor;

// Standard Entity Events
void WarpDoor_Update(void);
void WarpDoor_LateUpdate(void);
void WarpDoor_StaticUpdate(void);
void WarpDoor_Draw(void);
void WarpDoor_Create(void *data);
void WarpDoor_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void WarpDoor_EditorDraw(void);
void WarpDoor_EditorLoad(void);
#endif
void WarpDoor_Serialize(void);

// Extra Entity Functions
void WarpDoor_SetupPlayerCamera(void);
void WarpDoor_SetStageBounds(void);
void WarpDoor_SetWarpBounds(uint8 id);
void WarpDoor_CheckAllBounds(void);
bool32 WarpDoor_SetupBoundaries(int16 boundsID, Vector2 *posPtr);
void WarpDoor_DrawDebug(void);
void WarpDoor_SetupHitbox(void);

#endif //! OBJ_WARPDOOR_H
