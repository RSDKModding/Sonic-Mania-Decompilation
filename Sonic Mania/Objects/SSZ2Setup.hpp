#ifndef OBJ_SSZ2SETUP_H
#define OBJ_SSZ2SETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSSZ2Setup : Object {

};

// Entity Class
struct EntitySSZ2Setup : Entity {

};

// Object Entity
extern ObjectSSZ2Setup SSZ2Setup;

// Standard Entity Events
void SSZ2Setup_Update();
void SSZ2Setup_LateUpdate();
void SSZ2Setup_StaticUpdate();
void SSZ2Setup_Draw();
void SSZ2Setup_Create(void* data);
void SSZ2Setup_StageLoad();
void SSZ2Setup_EditorDraw();
void SSZ2Setup_EditorLoad();
void SSZ2Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_SSZ2SETUP_H
