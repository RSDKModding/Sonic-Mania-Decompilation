#ifndef OBJ_WOODCHIPPER_H
#define OBJ_WOODCHIPPER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectWoodChipper : Object {

};

// Entity Class
struct EntityWoodChipper : Entity {

};

// Object Entity
extern ObjectWoodChipper WoodChipper;

// Standard Entity Events
void WoodChipper_Update();
void WoodChipper_LateUpdate();
void WoodChipper_StaticUpdate();
void WoodChipper_Draw();
void WoodChipper_Create(void* data);
void WoodChipper_StageLoad();
void WoodChipper_EditorDraw();
void WoodChipper_EditorLoad();
void WoodChipper_Serialize();

// Extra Entity Functions


#endif //!OBJ_WOODCHIPPER_H
