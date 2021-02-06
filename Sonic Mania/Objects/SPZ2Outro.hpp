#ifndef OBJ_SPZ2OUTRO_H
#define OBJ_SPZ2OUTRO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSPZ2Outro : Object {

};

// Entity Class
struct EntitySPZ2Outro : Entity {

};

// Object Entity
extern ObjectSPZ2Outro SPZ2Outro;

// Standard Entity Events
void SPZ2Outro_Update();
void SPZ2Outro_LateUpdate();
void SPZ2Outro_StaticUpdate();
void SPZ2Outro_Draw();
void SPZ2Outro_Create(void* data);
void SPZ2Outro_StageLoad();
void SPZ2Outro_EditorDraw();
void SPZ2Outro_EditorLoad();
void SPZ2Outro_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPZ2OUTRO_H
