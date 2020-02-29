#ifndef OBJ_HANDLAUNCHER_H
#define OBJ_HANDLAUNCHER_H

#include "../../SonicMania.h"

//Object Class
class ObjectHandLauncher : public Object {
public:

};

//Entity Class
class EntityHandLauncher : public Entity {
public:

};

//Object Entity
ObjectHandLauncher HandLauncher;

//Entity Functions
void HandLauncher_Update();
void HandLauncher_EarlyUpdate();
void HandLauncher_LateUpdate();
void HandLauncher_Draw();
void HandLauncher_Setup(void* subtype);
void HandLauncher_StageLoad();
void HandLauncher_GetAttributes();

#endif //!OBJ_HANDLAUNCHER_H
