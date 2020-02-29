#ifndef OBJ_THANKSSETUP_H
#define OBJ_THANKSSETUP_H

//Object Class
class ThanksSetup : Object {

};

//Entity Class
class EntityThanksSetup : Entity {

};

//Entity Functions
void ThanksSetup_Update();
void ThanksSetup_EarlyUpdate();
void ThanksSetup_LateUpdate();
void ThanksSetup_Draw();
void ThanksSetup_Setup(void* subtype);
void ThanksSetup_StageLoad();
void ThanksSetup_GetAttributes();

#endif //!OBJ_THANKSSETUP_H
