#ifndef OBJ_TORNADOPATH_H
#define OBJ_TORNADOPATH_H

#include "../../SonicMania.h"

//Object Class
class ObjectTornadoPath : public Object {
public:

};

//Entity Class
class EntityTornadoPath : public Entity {
public:

};

//Object Entity
ObjectTornadoPath TornadoPath;

//Entity Functions
void TornadoPath_Update();
void TornadoPath_EarlyUpdate();
void TornadoPath_LateUpdate();
void TornadoPath_Draw();
void TornadoPath_Setup(void* subtype);
void TornadoPath_StageLoad();
void TornadoPath_GetAttributes();

#endif //!OBJ_TORNADOPATH_H
