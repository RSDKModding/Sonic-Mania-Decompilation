#ifndef OBJ_SPECIALCLEAR_H
#define OBJ_SPECIALCLEAR_H

#include "../../SonicMania.h"

//Object Class
class ObjectSpecialClear : public Object {
public:

};

//Entity Class
class EntitySpecialClear : public Entity {
public:

};

//Object Entity
ObjectSpecialClear SpecialClear;

//Entity Functions
void SpecialClear_Update();
void SpecialClear_EarlyUpdate();
void SpecialClear_LateUpdate();
void SpecialClear_Draw();
void SpecialClear_Setup(void* subtype);
void SpecialClear_StageLoad();
void SpecialClear_GetAttributes();

#endif //!OBJ_SPECIALCLEAR_H
