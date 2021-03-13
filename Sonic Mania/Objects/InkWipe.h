#ifndef OBJ_INKWIPE_H
#define OBJ_INKWIPE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectInkWipe;

// Entity Class
typedef struct {
    RSDK_ENTITY
    EntityAnimationData data;
} EntityInkWipe;

// Object Struct
extern ObjectInkWipe *InkWipe;

// Standard Entity Events
void InkWipe_Update();
void InkWipe_LateUpdate();
void InkWipe_StaticUpdate();
void InkWipe_Draw();
void InkWipe_Create(void* data);
void InkWipe_StageLoad();
void InkWipe_EditorDraw();
void InkWipe_EditorLoad();
void InkWipe_Serialize();

// Extra Entity Functions


#endif //!OBJ_INKWIPE_H
