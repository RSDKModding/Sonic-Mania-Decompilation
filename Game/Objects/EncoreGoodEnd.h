#ifndef OBJ_ENCOREGOODEND_H
#define OBJ_ENCOREGOODEND_H

#include "../../SonicMania.h"

//Object Class
class ObjectEncoreGoodEnd : public Object {
public:

};

//Entity Class
class EntityEncoreGoodEnd : public Entity {
public:

};

//Object Entity
ObjectEncoreGoodEnd EncoreGoodEnd;

//Entity Functions
void EncoreGoodEnd_Update();
void EncoreGoodEnd_EarlyUpdate();
void EncoreGoodEnd_LateUpdate();
void EncoreGoodEnd_Draw();
void EncoreGoodEnd_Setup(void* subtype);
void EncoreGoodEnd_StageLoad();
void EncoreGoodEnd_GetAttributes();

#endif //!OBJ_ENCOREGOODEND_H
