#ifndef OBJ_SPZ2SETUP_H
#define OBJ_SPZ2SETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSPZ2Setup : Object {

};

// Entity Class
struct EntitySPZ2Setup : Entity {

};

// Object Struct
extern ObjectSPZ2Setup SPZ2Setup;

// Standard Entity Events
void SPZ2Setup_Update();
void SPZ2Setup_LateUpdate();
void SPZ2Setup_StaticUpdate();
void SPZ2Setup_Draw();
void SPZ2Setup_Create(void* data);
void SPZ2Setup_StageLoad();
void SPZ2Setup_EditorDraw();
void SPZ2Setup_EditorLoad();
void SPZ2Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPZ2SETUP_H
