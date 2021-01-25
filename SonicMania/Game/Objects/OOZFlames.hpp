#ifndef OBJ_OOZFLAMES_H
#define OBJ_OOZFLAMES_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectOOZFlames : Object {

};

// Entity Class
struct EntityOOZFlames : Entity {

};

// Object Entity
extern ObjectOOZFlames OOZFlames;

// Standard Entity Events
void OOZFlames_Update();
void OOZFlames_LateUpdate();
void OOZFlames_StaticUpdate();
void OOZFlames_Draw();
void OOZFlames_Create(void* data);
void OOZFlames_StageLoad();
void OOZFlames_EditorDraw();
void OOZFlames_EditorLoad();
void OOZFlames_Serialize();

// Extra Entity Functions


#endif //!OBJ_OOZFLAMES_H
