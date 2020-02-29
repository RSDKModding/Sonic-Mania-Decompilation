#ifndef OBJ_MMZWHEEL_H
#define OBJ_MMZWHEEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectMMZWheel : public Object {
public:

};

//Entity Class
class EntityMMZWheel : public Entity {
public:

};

//Object Entity
ObjectMMZWheel MMZWheel;

//Entity Functions
void MMZWheel_Update();
void MMZWheel_EarlyUpdate();
void MMZWheel_LateUpdate();
void MMZWheel_Draw();
void MMZWheel_Setup(void* subtype);
void MMZWheel_StageLoad();
void MMZWheel_GetAttributes();

#endif //!OBJ_MMZWHEEL_H
