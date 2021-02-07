#ifndef OBJ_PSZ2OUTRO_H
#define OBJ_PSZ2OUTRO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPSZ2Outro : Object {

};

// Entity Class
struct EntityPSZ2Outro : Entity {

};

// Object Struct
extern ObjectPSZ2Outro PSZ2Outro;

// Standard Entity Events
void PSZ2Outro_Update();
void PSZ2Outro_LateUpdate();
void PSZ2Outro_StaticUpdate();
void PSZ2Outro_Draw();
void PSZ2Outro_Create(void* data);
void PSZ2Outro_StageLoad();
void PSZ2Outro_EditorDraw();
void PSZ2Outro_EditorLoad();
void PSZ2Outro_Serialize();

// Extra Entity Functions


#endif //!OBJ_PSZ2OUTRO_H
