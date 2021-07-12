#ifndef OBJ_WARPDOOR_H
#define OBJ_WARPDOOR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Entity *tags[256];
    Hitbox boundaries[256];
    byte boundCount;
    byte field_1405;
    byte field_1406;
    int field_1408;
    int field_140C;
    ushort aniFrames;
    ushort sfxRubyAttackL[6];
    ushort sfxRubyAttackR[6];
    ushort sfxWarpDoor;
} ObjectWarpDoor;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int go;
    int width;
    int height;
    byte tag;
    int destinationTag;
    bool32 definesBounds;
    int xBoundaryPosL;
    int xBoundaryPosR;
    int yBoundaryPosT;
    int yBoundaryPosB;
    byte effect;
    bool32 warpToCenter;
    byte forcePlayerState;
    Animator animator;
    int field_A4;
    byte field_A8;
    byte field_A9;
    byte field_AA;
    byte field_AB;
    ushort boundID;
    byte field_AE;
    byte field_AF;
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
void WarpDoor_Unknown3(byte id);
void WarpDoor_Unknown4(void);
bool32 WarpDoor_Unknown5(short a1, Vector2 *posPtr);
void WarpDoor_DrawDebug(void);
void WarpDoor_Unknown7(void);

#endif //!OBJ_WARPDOOR_H
