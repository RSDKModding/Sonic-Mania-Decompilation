#ifndef OBJ_BARSTOOL_H
#define OBJ_BARSTOOL_H

#include "../../SonicMania.h"

//Object Class
class ObjectBarStool : public Object {
public:

};

//Entity Class
class EntityBarStool : public Entity {
public:

};

//Object Entity
ObjectBarStool BarStool;

//Entity Functions
void BarStool_Update();
void BarStool_EarlyUpdate();
void BarStool_LateUpdate();
void BarStool_Draw();
void BarStool_Setup(void* subtype);
void BarStool_StageLoad();
void BarStool_GetAttributes();

#endif //!OBJ_BARSTOOL_H
