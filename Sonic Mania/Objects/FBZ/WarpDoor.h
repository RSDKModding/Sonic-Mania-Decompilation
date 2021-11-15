#ifndef OBJ_WARPDOOR_H
#define OBJ_WARPDOOR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Entity *tags[256];
    Hitbox boundaries[256];
    uint8 boundCount;
    uint8 field_1405;
    uint8 field_1406;
    int32 field_1408;
    int32 field_140C;
    uint16 aniFrames;
    uint16 sfxRubyAttackL[6];
    uint16 sfxRubyAttackR[6];
    uint16 sfxWarpDoor;
} ObjectWarpDoor;

// Entity Class
typedef struct {
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
    int32 field_A4;
    uint8 field_A8;
    uint8 field_A9;
    uint8 field_AA;
    uint8 field_AB;
    uint16 boundID;
    uint8 field_AE;
    uint8 field_AF;
    int32 fadeTimer;
    int32 field_B4;
    Hitbox hitbox;
} EntityWarpDoor;

// Object Struct
extern ObjectWarpDoor *WarpDoor;

// Standard Entity Events
void WarpDoor_Update(void);
void WarpDoor_LateUpdate(void);
void WarpDoor_StaticUpdate(void);
void WarpDoor_Draw(void);
void WarpDoor_Create(void* data);
void WarpDoor_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
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

#endif //!OBJ_WARPDOOR_H
