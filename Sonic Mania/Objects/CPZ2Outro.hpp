#ifndef OBJ_CPZ2OUTRO_H
#define OBJ_CPZ2OUTRO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCPZ2Outro : Object {

};

// Entity Class
struct EntityCPZ2Outro : Entity {

};

// Object Struct
extern ObjectCPZ2Outro CPZ2Outro;

// Standard Entity Events
void CPZ2Outro_Update();
void CPZ2Outro_LateUpdate();
void CPZ2Outro_StaticUpdate();
void CPZ2Outro_Draw();
void CPZ2Outro_Create(void* data);
void CPZ2Outro_StageLoad();
void CPZ2Outro_EditorDraw();
void CPZ2Outro_EditorLoad();
void CPZ2Outro_Serialize();

// Extra Entity Functions


#endif //!OBJ_CPZ2OUTRO_H
