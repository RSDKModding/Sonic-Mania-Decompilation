#ifndef OBJ_PRINTBLOCK_H
#define OBJ_PRINTBLOCK_H

#include "../../SonicMania.h"

//Object Class
class ObjectPrintBlock : public Object {
public:

};

//Entity Class
class EntityPrintBlock : public Entity {
public:

};

//Object Entity
ObjectPrintBlock PrintBlock;

//Entity Functions
void PrintBlock_Update();
void PrintBlock_EarlyUpdate();
void PrintBlock_LateUpdate();
void PrintBlock_Draw();
void PrintBlock_Setup(void* subtype);
void PrintBlock_StageLoad();
void PrintBlock_GetAttributes();

#endif //!OBJ_PRINTBLOCK_H
