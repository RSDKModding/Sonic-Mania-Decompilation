#ifndef OBJ_FLINGRAMP_H
#define OBJ_FLINGRAMP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFlingRamp : Object{
    Hitbox hitbox;
};

// Entity Class
struct EntityFlingRamp : Entity {

};

// Object Struct
extern ObjectFlingRamp *FlingRamp;

// Standard Entity Events
void FlingRamp_Update();
void FlingRamp_LateUpdate();
void FlingRamp_StaticUpdate();
void FlingRamp_Draw();
void FlingRamp_Create(void* data);
void FlingRamp_StageLoad();
void FlingRamp_EditorDraw();
void FlingRamp_EditorLoad();
void FlingRamp_Serialize();

// Extra Entity Functions


#endif //!OBJ_FLINGRAMP_H
