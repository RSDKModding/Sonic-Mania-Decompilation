#ifndef OBJ_MAGSPIKEBALL_H
#define OBJ_MAGSPIKEBALL_H

#include "../../SonicMania.h"

//Object Class
class ObjectMagSpikeBall : public Object {
public:

};

//Entity Class
class EntityMagSpikeBall : public Entity {
public:

};

//Object Entity
ObjectMagSpikeBall MagSpikeBall;

//Entity Functions
void MagSpikeBall_Update();
void MagSpikeBall_EarlyUpdate();
void MagSpikeBall_LateUpdate();
void MagSpikeBall_Draw();
void MagSpikeBall_Setup(void* subtype);
void MagSpikeBall_StageLoad();
void MagSpikeBall_GetAttributes();

#endif //!OBJ_MAGSPIKEBALL_H
