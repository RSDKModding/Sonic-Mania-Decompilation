#ifndef OBJ_MMZSETUP_H
#define OBJ_MMZSETUP_H

//Object Class
class MMZSetup : Object {

};

//Entity Class
class EntityMMZSetup : Entity {

};

//Entity Functions
void MMZSetup_Update();
void MMZSetup_EarlyUpdate();
void MMZSetup_LateUpdate();
void MMZSetup_Draw();
void MMZSetup_Setup(void* subtype);
void MMZSetup_StageLoad();
void MMZSetup_GetAttributes();

#endif //!OBJ_MMZSETUP_H
