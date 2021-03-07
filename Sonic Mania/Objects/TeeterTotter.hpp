#ifndef OBJ_TEETERTOTTER_H
#define OBJ_TEETERTOTTER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTeeterTotter : Object {

};

// Entity Class
struct EntityTeeterTotter : Entity {

};

// Object Struct
extern ObjectTeeterTotter *TeeterTotter;

// Standard Entity Events
void TeeterTotter_Update();
void TeeterTotter_LateUpdate();
void TeeterTotter_StaticUpdate();
void TeeterTotter_Draw();
void TeeterTotter_Create(void* data);
void TeeterTotter_StageLoad();
void TeeterTotter_EditorDraw();
void TeeterTotter_EditorLoad();
void TeeterTotter_Serialize();

// Extra Entity Functions


#endif //!OBJ_TEETERTOTTER_H
