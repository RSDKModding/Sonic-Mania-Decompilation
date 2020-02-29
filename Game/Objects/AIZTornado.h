#ifndef OBJ_AIZTORNADO_H
#define OBJ_AIZTORNADO_H

#include "../../SonicMania.h"

//Object Class
class ObjectAIZTornado : public Object {
public:

};

//Entity Class
class EntityAIZTornado : public Entity {
public:

};

//Object Entity
ObjectAIZTornado AIZTornado;

//Entity Functions
void AIZTornado_Update();
void AIZTornado_EarlyUpdate();
void AIZTornado_LateUpdate();
void AIZTornado_Draw();
void AIZTornado_Setup(void* subtype);
void AIZTornado_StageLoad();
void AIZTornado_GetAttributes();

#endif //!OBJ_AIZTORNADO_H
