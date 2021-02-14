#ifndef OBJ_INKWIPE_H
#define OBJ_INKWIPE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectInkWipe : Object{
    ushort spriteIndex;
};

// Entity Class
struct EntityInkWipe : Entity {
    EntityAnimationData data;
};

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
