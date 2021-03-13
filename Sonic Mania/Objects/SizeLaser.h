#ifndef OBJ_SIZELASER_H
#define OBJ_SIZELASER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox playerOuterBox[4];
    Hitbox playerInnerBox[4];
    Vector2 playerPositions[4];
    ushort spriteIndex;
    ushort sonicIndex;
    ushort tailsIndex;
    ushort tailSpriteIndex;
    ushort knuxIndex;
    ushort mightyIndex;
    ushort rayIndex;
    ushort sfx_Shrink2;
    ushort sfx_Grow2;
} ObjectSizeLaser;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    byte type;
    byte orientation;
    int timer;
    Vector2 storedPos;
    int extend;
    EntityAnimationData animData1;
    EntityAnimationData animData2;
} EntitySizeLaser;

// Object Struct
extern ObjectSizeLaser *SizeLaser;

// Standard Entity Events
void SizeLaser_Update();
void SizeLaser_LateUpdate();
void SizeLaser_StaticUpdate();
void SizeLaser_Draw();
void SizeLaser_Create(void* data);
void SizeLaser_StageLoad();
void SizeLaser_EditorDraw();
void SizeLaser_EditorLoad();
void SizeLaser_Serialize();

// Extra Entity Functions
bool32 SizeLaser_SizeChangeSFXCheck();

#endif //!OBJ_SIZELASER_H
