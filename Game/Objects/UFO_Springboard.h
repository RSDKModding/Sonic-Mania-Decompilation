#ifndef OBJ_UFO_SPRINGBOARD_H
#define OBJ_UFO_SPRINGBOARD_H

#include "../../SonicMania.h"

//Object Class
class ObjectUFO_Springboard : public Object {
public:

};

//Entity Class
class EntityUFO_Springboard : public Entity {
public:

};

//Object Entity
ObjectUFO_Springboard UFO_Springboard;

//Entity Functions
void UFO_Springboard_Update();
void UFO_Springboard_EarlyUpdate();
void UFO_Springboard_LateUpdate();
void UFO_Springboard_Draw();
void UFO_Springboard_Setup(void* subtype);
void UFO_Springboard_StageLoad();
void UFO_Springboard_GetAttributes();

#endif //!OBJ_UFO_SPRINGBOARD_H
