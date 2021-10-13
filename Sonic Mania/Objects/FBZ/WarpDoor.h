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
    int field_1408;
    int field_140C;
    uint16 aniFrames;
    uint16 sfxRubyAttackL[6];
    uint16 sfxRubyAttackR[6];
    uint16 sfxWarpDoor;
} ObjectWarpDoor;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int go;
    int width;
    int height;
    uint8 tag;
    int destinationTag;
    bool32 definesBounds;
    int xBoundaryPosL;
    int xBoundaryPosR;
    int yBoundaryPosT;
    int yBoundaryPosB;
    uint8 effect;
    bool32 warpToCenter;
    uint8 forcePlayerState;
    Animator animator;
    int field_A4;
    uint8 field_A8;
    uint8 field_A9;
    uint8 field_AA;
    uint8 field_AB;
    uint16 boundID;
    uint8 field_AE;
    uint8 field_AF;
    int fadeTimer;
    int field_B4;
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
void WarpDoor_EditorDraw(void);
void WarpDoor_EditorLoad(void);
void WarpDoor_Serialize(void);

// Extra Entity Functions
void WarpDoor_Unknown1(void);
void WarpDoor_Unknown2(void);
void WarpDoor_Unknown3(uint8 id);
void WarpDoor_Unknown4(void);
bool32 WarpDoor_Unknown5(int16 a1, Vector2 *posPtr);
void WarpDoor_DrawDebug(void);
void WarpDoor_Unknown7(void);

#endif //!OBJ_WARPDOOR_H
