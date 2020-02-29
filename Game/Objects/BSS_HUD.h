#ifndef OBJ_BSS_HUD_H
#define OBJ_BSS_HUD_H

#include "../../SonicMania.h"

//Object Class
class ObjectBSS_HUD : public Object {
public:

};

//Entity Class
class EntityBSS_HUD : public Entity {
public:

};

//Object Entity
ObjectBSS_HUD BSS_HUD;

//Entity Functions
void BSS_HUD_Update();
void BSS_HUD_EarlyUpdate();
void BSS_HUD_LateUpdate();
void BSS_HUD_Draw();
void BSS_HUD_Setup(void* subtype);
void BSS_HUD_StageLoad();
void BSS_HUD_GetAttributes();

#endif //!OBJ_BSS_HUD_H
