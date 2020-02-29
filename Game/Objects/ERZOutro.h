#ifndef OBJ_ERZOUTRO_H
#define OBJ_ERZOUTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectERZOutro : public Object {
public:

};

//Entity Class
class EntityERZOutro : public Entity {
public:

};

//Object Entity
ObjectERZOutro ERZOutro;

//Entity Functions
void ERZOutro_Update();
void ERZOutro_EarlyUpdate();
void ERZOutro_LateUpdate();
void ERZOutro_Draw();
void ERZOutro_Setup(void* subtype);
void ERZOutro_StageLoad();
void ERZOutro_GetAttributes();

#endif //!OBJ_ERZOUTRO_H
