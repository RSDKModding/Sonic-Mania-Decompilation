#ifndef OBJ_PSZLAUNCHER_H
#define OBJ_PSZLAUNCHER_H

#include "../../SonicMania.h"

//Object Class
class ObjectPSZLauncher : public Object {
public:

};

//Entity Class
class EntityPSZLauncher : public Entity {
public:

};

//Object Entity
ObjectPSZLauncher PSZLauncher;

//Entity Functions
void PSZLauncher_Update();
void PSZLauncher_EarlyUpdate();
void PSZLauncher_LateUpdate();
void PSZLauncher_Draw();
void PSZLauncher_Setup(void* subtype);
void PSZLauncher_StageLoad();
void PSZLauncher_GetAttributes();

#endif //!OBJ_PSZLAUNCHER_H
