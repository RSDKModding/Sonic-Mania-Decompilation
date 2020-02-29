#ifndef OBJ_HILOSIGN_H
#define OBJ_HILOSIGN_H

#include "../../SonicMania.h"

//Object Class
class ObjectHiLoSign : public Object {
public:

};

//Entity Class
class EntityHiLoSign : public Entity {
public:

};

//Object Entity
ObjectHiLoSign HiLoSign;

//Entity Functions
void HiLoSign_Update();
void HiLoSign_EarlyUpdate();
void HiLoSign_LateUpdate();
void HiLoSign_Draw();
void HiLoSign_Setup(void* subtype);
void HiLoSign_StageLoad();
void HiLoSign_GetAttributes();

#endif //!OBJ_HILOSIGN_H
