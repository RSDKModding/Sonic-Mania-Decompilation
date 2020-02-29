#ifndef OBJ_MMZSETUP_H
#define OBJ_MMZSETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectMMZSetup : public Object {
public:

};

//Entity Class
class EntityMMZSetup : public Entity {
public:

};

//Object Entity
ObjectMMZSetup MMZSetup;

//Entity Functions
void MMZSetup_Update();
void MMZSetup_EarlyUpdate();
void MMZSetup_LateUpdate();
void MMZSetup_Draw();
void MMZSetup_Setup(void* subtype);
void MMZSetup_StageLoad();
void MMZSetup_GetAttributes();

#endif //!OBJ_MMZSETUP_H
