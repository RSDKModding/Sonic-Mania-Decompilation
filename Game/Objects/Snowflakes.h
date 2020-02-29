#ifndef OBJ_SNOWFLAKES_H
#define OBJ_SNOWFLAKES_H

#include "../../SonicMania.h"

//Object Class
class ObjectSnowflakes : public Object {
public:

};

//Entity Class
class EntitySnowflakes : public Entity {
public:

};

//Object Entity
ObjectSnowflakes Snowflakes;

//Entity Functions
void Snowflakes_Update();
void Snowflakes_EarlyUpdate();
void Snowflakes_LateUpdate();
void Snowflakes_Draw();
void Snowflakes_Setup(void* subtype);
void Snowflakes_StageLoad();
void Snowflakes_GetAttributes();

#endif //!OBJ_SNOWFLAKES_H
