#ifndef OBJ_RINGFIELD_H
#define OBJ_RINGFIELD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectRingField : Object {

};

// Entity Class
struct EntityRingField : Entity {

};

// Object Entity
extern ObjectRingField RingField;

// Standard Entity Events
void RingField_Update();
void RingField_LateUpdate();
void RingField_StaticUpdate();
void RingField_Draw();
void RingField_Create(void* data);
void RingField_StageLoad();
void RingField_EditorDraw();
void RingField_EditorLoad();
void RingField_Serialize();

// Extra Entity Functions


#endif //!OBJ_RINGFIELD_H
