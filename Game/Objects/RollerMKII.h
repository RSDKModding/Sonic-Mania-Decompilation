#ifndef OBJ_ROLLERMKII_H
#define OBJ_ROLLERMKII_H

#include "../../SonicMania.h"

//Object Class
class ObjectRollerMKII : public Object {
public:

};

//Entity Class
class EntityRollerMKII : public Entity {
public:

};

//Object Entity
ObjectRollerMKII RollerMKII;

//Entity Functions
void RollerMKII_Update();
void RollerMKII_EarlyUpdate();
void RollerMKII_LateUpdate();
void RollerMKII_Draw();
void RollerMKII_Setup(void* subtype);
void RollerMKII_StageLoad();
void RollerMKII_GetAttributes();

#endif //!OBJ_ROLLERMKII_H
