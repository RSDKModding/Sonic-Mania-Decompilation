#ifndef OBJ_ROCKEMSOCKEM_H
#define OBJ_ROCKEMSOCKEM_H

#include "../../SonicMania.h"

//Object Class
class ObjectRockemSockem : public Object {
public:

};

//Entity Class
class EntityRockemSockem : public Entity {
public:

};

//Object Entity
ObjectRockemSockem RockemSockem;

//Entity Functions
void RockemSockem_Update();
void RockemSockem_EarlyUpdate();
void RockemSockem_LateUpdate();
void RockemSockem_Draw();
void RockemSockem_Setup(void* subtype);
void RockemSockem_StageLoad();
void RockemSockem_GetAttributes();

#endif //!OBJ_ROCKEMSOCKEM_H
