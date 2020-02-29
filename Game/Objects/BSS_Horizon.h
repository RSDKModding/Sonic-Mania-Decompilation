#ifndef OBJ_BSS_HORIZON_H
#define OBJ_BSS_HORIZON_H

#include "../../SonicMania.h"

//Object Class
class ObjectBSS_Horizon : public Object {
public:

};

//Entity Class
class EntityBSS_Horizon : public Entity {
public:

};

//Object Entity
ObjectBSS_Horizon BSS_Horizon;

//Entity Functions
void BSS_Horizon_Update();
void BSS_Horizon_EarlyUpdate();
void BSS_Horizon_LateUpdate();
void BSS_Horizon_Draw();
void BSS_Horizon_Setup(void* subtype);
void BSS_Horizon_StageLoad();
void BSS_Horizon_GetAttributes();

#endif //!OBJ_BSS_HORIZON_H
