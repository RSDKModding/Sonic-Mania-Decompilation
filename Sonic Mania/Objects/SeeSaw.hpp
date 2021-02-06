#ifndef OBJ_SEESAW_H
#define OBJ_SEESAW_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSeeSaw : Object {

};

// Entity Class
struct EntitySeeSaw : Entity {

};

// Object Entity
extern ObjectSeeSaw SeeSaw;

// Standard Entity Events
void SeeSaw_Update();
void SeeSaw_LateUpdate();
void SeeSaw_StaticUpdate();
void SeeSaw_Draw();
void SeeSaw_Create(void* data);
void SeeSaw_StageLoad();
void SeeSaw_EditorDraw();
void SeeSaw_EditorLoad();
void SeeSaw_Serialize();

// Extra Entity Functions


#endif //!OBJ_SEESAW_H
