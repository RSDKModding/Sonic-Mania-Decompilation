#ifndef OBJ_RTELEPORTER_H
#define OBJ_RTELEPORTER_H

#include "../../SonicMania.h"

//Object Class
class ObjectRTeleporter : public Object {
public:

};

//Entity Class
class EntityRTeleporter : public Entity {
public:

};

//Object Entity
ObjectRTeleporter RTeleporter;

//Entity Functions
void RTeleporter_Update();
void RTeleporter_EarlyUpdate();
void RTeleporter_LateUpdate();
void RTeleporter_Draw();
void RTeleporter_Setup(void* subtype);
void RTeleporter_StageLoad();
void RTeleporter_GetAttributes();

#endif //!OBJ_RTELEPORTER_H
