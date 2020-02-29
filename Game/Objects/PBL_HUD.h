#ifndef OBJ_PBL_HUD_H
#define OBJ_PBL_HUD_H

#include "../../SonicMania.h"

//Object Class
class ObjectPBL_HUD : public Object {
public:

};

//Entity Class
class EntityPBL_HUD : public Entity {
public:

};

//Object Entity
ObjectPBL_HUD PBL_HUD;

//Entity Functions
void PBL_HUD_Update();
void PBL_HUD_EarlyUpdate();
void PBL_HUD_LateUpdate();
void PBL_HUD_Draw();
void PBL_HUD_Setup(void* subtype);
void PBL_HUD_StageLoad();
void PBL_HUD_GetAttributes();

#endif //!OBJ_PBL_HUD_H
