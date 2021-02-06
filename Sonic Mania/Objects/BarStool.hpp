#ifndef OBJ_BARSTOOL_H
#define OBJ_BARSTOOL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBarStool : Object {

};

// Entity Class
struct EntityBarStool : Entity {

};

// Object Entity
extern ObjectBarStool BarStool;

// Standard Entity Events
void BarStool_Update();
void BarStool_LateUpdate();
void BarStool_StaticUpdate();
void BarStool_Draw();
void BarStool_Create(void* data);
void BarStool_StageLoad();
void BarStool_EditorDraw();
void BarStool_EditorLoad();
void BarStool_Serialize();

// Extra Entity Functions


#endif //!OBJ_BARSTOOL_H
