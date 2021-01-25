#ifndef OBJ_SIZELASER_H
#define OBJ_SIZELASER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSizeLaser : Object {

};

// Entity Class
struct EntitySizeLaser : Entity {

};

// Object Entity
extern ObjectSizeLaser SizeLaser;

// Standard Entity Events
void SizeLaser_Update();
void SizeLaser_LateUpdate();
void SizeLaser_StaticUpdate();
void SizeLaser_Draw();
void SizeLaser_Create(void* data);
void SizeLaser_StageLoad();
void SizeLaser_EditorDraw();
void SizeLaser_EditorLoad();
void SizeLaser_Serialize();

// Extra Entity Functions


#endif //!OBJ_SIZELASER_H
