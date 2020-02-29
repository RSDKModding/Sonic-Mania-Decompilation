#ifndef OBJ_YOYOPULLEY_H
#define OBJ_YOYOPULLEY_H

#include "../../SonicMania.h"

//Object Class
class ObjectYoyoPulley : public Object {
public:

};

//Entity Class
class EntityYoyoPulley : public Entity {
public:

};

//Object Entity
ObjectYoyoPulley YoyoPulley;

//Entity Functions
void YoyoPulley_Update();
void YoyoPulley_EarlyUpdate();
void YoyoPulley_LateUpdate();
void YoyoPulley_Draw();
void YoyoPulley_Setup(void* subtype);
void YoyoPulley_StageLoad();
void YoyoPulley_GetAttributes();

#endif //!OBJ_YOYOPULLEY_H
