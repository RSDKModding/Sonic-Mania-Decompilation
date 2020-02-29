#ifndef OBJ_ERZSTART_H
#define OBJ_ERZSTART_H

#include "../../SonicMania.h"

//Object Class
class ObjectERZStart : public Object {
public:

};

//Entity Class
class EntityERZStart : public Entity {
public:

};

//Object Entity
ObjectERZStart ERZStart;

//Entity Functions
void ERZStart_Update();
void ERZStart_EarlyUpdate();
void ERZStart_LateUpdate();
void ERZStart_Draw();
void ERZStart_Setup(void* subtype);
void ERZStart_StageLoad();
void ERZStart_GetAttributes();

#endif //!OBJ_ERZSTART_H
