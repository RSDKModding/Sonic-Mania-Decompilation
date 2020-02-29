#ifndef OBJ_MENUPARAM_H
#define OBJ_MENUPARAM_H

#include "../../SonicMania.h"

//Object Class
class ObjectMenuParam : public Object {
public:

};

//Entity Class
class EntityMenuParam : public Entity {
public:

};

//Object Entity
ObjectMenuParam MenuParam;

//Entity Functions
void MenuParam_Update();
void MenuParam_EarlyUpdate();
void MenuParam_LateUpdate();
void MenuParam_Draw();
void MenuParam_Setup(void* subtype);
void MenuParam_StageLoad();
void MenuParam_GetAttributes();

#endif //!OBJ_MENUPARAM_H
