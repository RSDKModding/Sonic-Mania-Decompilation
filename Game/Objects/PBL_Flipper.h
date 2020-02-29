#ifndef OBJ_PBL_FLIPPER_H
#define OBJ_PBL_FLIPPER_H

#include "../../SonicMania.h"

//Object Class
class ObjectPBL_Flipper : public Object {
public:

};

//Entity Class
class EntityPBL_Flipper : public Entity {
public:

};

//Object Entity
ObjectPBL_Flipper PBL_Flipper;

//Entity Functions
void PBL_Flipper_Update();
void PBL_Flipper_EarlyUpdate();
void PBL_Flipper_LateUpdate();
void PBL_Flipper_Draw();
void PBL_Flipper_Setup(void* subtype);
void PBL_Flipper_StageLoad();
void PBL_Flipper_GetAttributes();

#endif //!OBJ_PBL_FLIPPER_H
