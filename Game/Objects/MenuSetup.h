#ifndef OBJ_MENUSETUP_H
#define OBJ_MENUSETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectMenuSetup : public Object {
public:

};

//Entity Class
class EntityMenuSetup : public Entity {
public:

};

//Object Entity
ObjectMenuSetup MenuSetup;

//Entity Functions
void MenuSetup_Update();
void MenuSetup_EarlyUpdate();
void MenuSetup_LateUpdate();
void MenuSetup_Draw();
void MenuSetup_Setup(void* subtype);
void MenuSetup_StageLoad();
void MenuSetup_GetAttributes();

#endif //!OBJ_MENUSETUP_H
