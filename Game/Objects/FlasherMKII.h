#ifndef OBJ_FLASHERMKII_H
#define OBJ_FLASHERMKII_H

#include "../../SonicMania.h"

//Object Class
class ObjectFlasherMKII : public Object {
public:

};

//Entity Class
class EntityFlasherMKII : public Entity {
public:

};

//Object Entity
ObjectFlasherMKII FlasherMKII;

//Entity Functions
void FlasherMKII_Update();
void FlasherMKII_EarlyUpdate();
void FlasherMKII_LateUpdate();
void FlasherMKII_Draw();
void FlasherMKII_Setup(void* subtype);
void FlasherMKII_StageLoad();
void FlasherMKII_GetAttributes();

#endif //!OBJ_FLASHERMKII_H
