#ifndef OBJ_WATER_H
#define OBJ_WATER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectWater : Object {

};

// Entity Class
struct EntityWater : Entity {

};

// Object Entity
extern ObjectWater Water;

// Standard Entity Events
void Water_Update();
void Water_LateUpdate();
void Water_StaticUpdate();
void Water_Draw();
void Water_Create(void* data);
void Water_StageLoad();
void Water_EditorDraw();
void Water_EditorLoad();
void Water_Serialize();

// Extra Entity Functions


#endif //!OBJ_WATER_H
