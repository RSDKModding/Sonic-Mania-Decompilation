#ifndef OBJ_MSHOLOGRAM_H
#define OBJ_MSHOLOGRAM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMSHologram : Object{

};

// Entity Class
struct EntityMSHologram : Entity {

};

// Object Struct
extern ObjectMSHologram *MSHologram;

// Standard Entity Events
void MSHologram_Update();
void MSHologram_LateUpdate();
void MSHologram_StaticUpdate();
void MSHologram_Draw();
void MSHologram_Create(void* data);
void MSHologram_StageLoad();
void MSHologram_EditorDraw();
void MSHologram_EditorLoad();
void MSHologram_Serialize();

// Extra Entity Functions


#endif //!OBJ_MSHOLOGRAM_H
