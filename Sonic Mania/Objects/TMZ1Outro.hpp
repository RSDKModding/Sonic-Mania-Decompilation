#ifndef OBJ_TMZ1OUTRO_H
#define OBJ_TMZ1OUTRO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTMZ1Outro : Object {

};

// Entity Class
struct EntityTMZ1Outro : Entity {

};

// Object Struct
extern ObjectTMZ1Outro TMZ1Outro;

// Standard Entity Events
void TMZ1Outro_Update();
void TMZ1Outro_LateUpdate();
void TMZ1Outro_StaticUpdate();
void TMZ1Outro_Draw();
void TMZ1Outro_Create(void* data);
void TMZ1Outro_StageLoad();
void TMZ1Outro_EditorDraw();
void TMZ1Outro_EditorLoad();
void TMZ1Outro_Serialize();

// Extra Entity Functions


#endif //!OBJ_TMZ1OUTRO_H
