#ifndef OBJ_PATHINVERTER_H
#define OBJ_PATHINVERTER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPathInverter : Object{

};

// Entity Class
struct EntityPathInverter : Entity {

};

// Object Struct
extern ObjectPathInverter *PathInverter;

// Standard Entity Events
void PathInverter_Update();
void PathInverter_LateUpdate();
void PathInverter_StaticUpdate();
void PathInverter_Draw();
void PathInverter_Create(void* data);
void PathInverter_StageLoad();
void PathInverter_EditorDraw();
void PathInverter_EditorLoad();
void PathInverter_Serialize();

// Extra Entity Functions


#endif //!OBJ_PATHINVERTER_H
