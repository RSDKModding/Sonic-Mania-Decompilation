#ifndef OBJ_OOZ1OUTRO_H
#define OBJ_OOZ1OUTRO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectOOZ1Outro : Object {

};

// Entity Class
struct EntityOOZ1Outro : Entity {

};

// Object Struct
extern ObjectOOZ1Outro OOZ1Outro;

// Standard Entity Events
void OOZ1Outro_Update();
void OOZ1Outro_LateUpdate();
void OOZ1Outro_StaticUpdate();
void OOZ1Outro_Draw();
void OOZ1Outro_Create(void* data);
void OOZ1Outro_StageLoad();
void OOZ1Outro_EditorDraw();
void OOZ1Outro_EditorLoad();
void OOZ1Outro_Serialize();

// Extra Entity Functions


#endif //!OBJ_OOZ1OUTRO_H
