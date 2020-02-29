#ifndef OBJ_MICDROP_H
#define OBJ_MICDROP_H

#include "../../SonicMania.h"

//Object Class
class ObjectMicDrop : public Object {
public:

};

//Entity Class
class EntityMicDrop : public Entity {
public:

};

//Object Entity
ObjectMicDrop MicDrop;

//Entity Functions
void MicDrop_Update();
void MicDrop_EarlyUpdate();
void MicDrop_LateUpdate();
void MicDrop_Draw();
void MicDrop_Setup(void* subtype);
void MicDrop_StageLoad();
void MicDrop_GetAttributes();

#endif //!OBJ_MICDROP_H
