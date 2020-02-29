#ifndef OBJ_UFO_DECORATION_H
#define OBJ_UFO_DECORATION_H

#include "../../SonicMania.h"

//Object Class
class ObjectUFO_Decoration : public Object {
public:

};

//Entity Class
class EntityUFO_Decoration : public Entity {
public:

};

//Object Entity
ObjectUFO_Decoration UFO_Decoration;

//Entity Functions
void UFO_Decoration_Update();
void UFO_Decoration_EarlyUpdate();
void UFO_Decoration_LateUpdate();
void UFO_Decoration_Draw();
void UFO_Decoration_Setup(void* subtype);
void UFO_Decoration_StageLoad();
void UFO_Decoration_GetAttributes();

#endif //!OBJ_UFO_DECORATION_H
