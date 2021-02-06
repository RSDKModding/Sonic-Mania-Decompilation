#ifndef OBJ_THANKSSETUP_H
#define OBJ_THANKSSETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectThanksSetup : Object {

};

// Entity Class
struct EntityThanksSetup : Entity {

};

// Object Entity
extern ObjectThanksSetup ThanksSetup;

// Standard Entity Events
void ThanksSetup_Update();
void ThanksSetup_LateUpdate();
void ThanksSetup_StaticUpdate();
void ThanksSetup_Draw();
void ThanksSetup_Create(void* data);
void ThanksSetup_StageLoad();
void ThanksSetup_EditorDraw();
void ThanksSetup_EditorLoad();
void ThanksSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_THANKSSETUP_H
