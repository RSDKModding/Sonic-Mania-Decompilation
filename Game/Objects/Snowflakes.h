#ifndef OBJ_SNOWFLAKES_H
#define OBJ_SNOWFLAKES_H

//Object Class
class Snowflakes : Object {

};

//Entity Class
class EntitySnowflakes : Entity {

};

//Entity Functions
void Snowflakes_Update();
void Snowflakes_EarlyUpdate();
void Snowflakes_LateUpdate();
void Snowflakes_Draw();
void Snowflakes_Setup(void* subtype);
void Snowflakes_StageLoad();
void Snowflakes_GetAttributes();

#endif //!OBJ_SNOWFLAKES_H
