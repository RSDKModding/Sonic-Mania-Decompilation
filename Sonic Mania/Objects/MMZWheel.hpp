#ifndef OBJ_MMZWHEEL_H
#define OBJ_MMZWHEEL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMMZWheel : Object {

};

// Entity Class
struct EntityMMZWheel : Entity {

};

// Object Struct
extern ObjectMMZWheel *MMZWheel;

// Standard Entity Events
void MMZWheel_Update();
void MMZWheel_LateUpdate();
void MMZWheel_StaticUpdate();
void MMZWheel_Draw();
void MMZWheel_Create(void* data);
void MMZWheel_StageLoad();
void MMZWheel_EditorDraw();
void MMZWheel_EditorLoad();
void MMZWheel_Serialize();

// Extra Entity Functions


#endif //!OBJ_MMZWHEEL_H
