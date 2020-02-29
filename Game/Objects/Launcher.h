#ifndef OBJ_LAUNCHER_H
#define OBJ_LAUNCHER_H

#include "../../SonicMania.h"

//Object Class
class ObjectLauncher : public Object {
public:

};

//Entity Class
class EntityLauncher : public Entity {
public:

};

//Object Entity
ObjectLauncher Launcher;

//Entity Functions
void Launcher_Update();
void Launcher_EarlyUpdate();
void Launcher_LateUpdate();
void Launcher_Draw();
void Launcher_Setup(void* subtype);
void Launcher_StageLoad();
void Launcher_GetAttributes();

#endif //!OBJ_LAUNCHER_H
