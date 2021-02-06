#ifndef OBJ_KLEPTOMOBILE_H
#define OBJ_KLEPTOMOBILE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectKleptoMobile : Object {

};

// Entity Class
struct EntityKleptoMobile : Entity {

};

// Object Entity
extern ObjectKleptoMobile KleptoMobile;

// Standard Entity Events
void KleptoMobile_Update();
void KleptoMobile_LateUpdate();
void KleptoMobile_StaticUpdate();
void KleptoMobile_Draw();
void KleptoMobile_Create(void* data);
void KleptoMobile_StageLoad();
void KleptoMobile_EditorDraw();
void KleptoMobile_EditorLoad();
void KleptoMobile_Serialize();

// Extra Entity Functions


#endif //!OBJ_KLEPTOMOBILE_H
