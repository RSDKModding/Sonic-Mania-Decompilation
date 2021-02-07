#ifndef OBJ_SPECIALRING_H
#define OBJ_SPECIALRING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpecialRing : Object{

};

// Entity Class
struct EntitySpecialRing : Entity {

};

// Object Struct
extern ObjectSpecialRing *SpecialRing;

// Standard Entity Events
void SpecialRing_Update();
void SpecialRing_LateUpdate();
void SpecialRing_StaticUpdate();
void SpecialRing_Draw();
void SpecialRing_Create(void* data);
void SpecialRing_StageLoad();
void SpecialRing_EditorDraw();
void SpecialRing_EditorLoad();
void SpecialRing_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPECIALRING_H
