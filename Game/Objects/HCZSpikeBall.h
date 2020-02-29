#ifndef OBJ_HCZSPIKEBALL_H
#define OBJ_HCZSPIKEBALL_H

#include "../../SonicMania.h"

//Object Class
class ObjectHCZSpikeBall : public Object {
public:

};

//Entity Class
class EntityHCZSpikeBall : public Entity {
public:

};

//Object Entity
ObjectHCZSpikeBall HCZSpikeBall;

//Entity Functions
void HCZSpikeBall_Update();
void HCZSpikeBall_EarlyUpdate();
void HCZSpikeBall_LateUpdate();
void HCZSpikeBall_Draw();
void HCZSpikeBall_Setup(void* subtype);
void HCZSpikeBall_StageLoad();
void HCZSpikeBall_GetAttributes();

#endif //!OBJ_HCZSPIKEBALL_H
