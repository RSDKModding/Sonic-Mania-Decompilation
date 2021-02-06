#ifndef OBJ_TMZ2OUTRO_H
#define OBJ_TMZ2OUTRO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTMZ2Outro : Object {

};

// Entity Class
struct EntityTMZ2Outro : Entity {

};

// Object Entity
extern ObjectTMZ2Outro TMZ2Outro;

// Standard Entity Events
void TMZ2Outro_Update();
void TMZ2Outro_LateUpdate();
void TMZ2Outro_StaticUpdate();
void TMZ2Outro_Draw();
void TMZ2Outro_Create(void* data);
void TMZ2Outro_StageLoad();
void TMZ2Outro_EditorDraw();
void TMZ2Outro_EditorLoad();
void TMZ2Outro_Serialize();

// Extra Entity Functions


#endif //!OBJ_TMZ2OUTRO_H
