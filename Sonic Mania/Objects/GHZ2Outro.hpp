#ifndef OBJ_GHZ2OUTRO_H
#define OBJ_GHZ2OUTRO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectGHZ2Outro : Object {

};

// Entity Class
struct EntityGHZ2Outro : Entity {

};

// Object Struct
extern ObjectGHZ2Outro *GHZ2Outro;

// Standard Entity Events
void GHZ2Outro_Update();
void GHZ2Outro_LateUpdate();
void GHZ2Outro_StaticUpdate();
void GHZ2Outro_Draw();
void GHZ2Outro_Create(void* data);
void GHZ2Outro_StageLoad();
void GHZ2Outro_EditorDraw();
void GHZ2Outro_EditorLoad();
void GHZ2Outro_Serialize();

// Extra Entity Functions


#endif //!OBJ_GHZ2OUTRO_H
