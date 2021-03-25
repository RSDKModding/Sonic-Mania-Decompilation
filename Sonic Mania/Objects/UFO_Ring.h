#ifndef OBJ_UFO_RING_H
#define OBJ_UFO_RING_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int field_8;
    ushort spriteIndex;
    ushort sfx_Ring;
} ObjectUFO_Ring;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    int field_5C;
    int height;
    int field_64;
    int worldX;
    int worldY;
    AnimationData data;
} EntityUFO_Ring;

// Object Struct
extern ObjectUFO_Ring *UFO_Ring;

// Standard Entity Events
void UFO_Ring_Update();
void UFO_Ring_LateUpdate();
void UFO_Ring_StaticUpdate();
void UFO_Ring_Draw();
void UFO_Ring_Create(void* data);
void UFO_Ring_StageLoad();
void UFO_Ring_EditorDraw();
void UFO_Ring_EditorLoad();
void UFO_Ring_Serialize();

// Extra Entity Functions
void UFO_Ring_PlayRingSFX();
void UFO_Ring_LoseRings(EntityUFO_Player *player);
//States
void UFO_Ring_Unknown2();
void UFO_Ring_Unknown3();
void UFO_Ring_Unknown4();

#endif //!OBJ_UFO_RING_H
