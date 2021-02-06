#ifndef OBJ_PSZ2SETUP_H
#define OBJ_PSZ2SETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPSZ2Setup : Object {

};

// Entity Class
struct EntityPSZ2Setup : Entity {

};

// Object Entity
extern ObjectPSZ2Setup PSZ2Setup;

// Standard Entity Events
void PSZ2Setup_Update();
void PSZ2Setup_LateUpdate();
void PSZ2Setup_StaticUpdate();
void PSZ2Setup_Draw();
void PSZ2Setup_Create(void* data);
void PSZ2Setup_StageLoad();
void PSZ2Setup_EditorDraw();
void PSZ2Setup_EditorLoad();
void PSZ2Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_PSZ2SETUP_H
