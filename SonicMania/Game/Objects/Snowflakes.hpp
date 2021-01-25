#ifndef OBJ_SNOWFLAKES_H
#define OBJ_SNOWFLAKES_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSnowflakes : Object {

};

// Entity Class
struct EntitySnowflakes : Entity {

};

// Object Entity
extern ObjectSnowflakes Snowflakes;

// Standard Entity Events
void Snowflakes_Update();
void Snowflakes_LateUpdate();
void Snowflakes_StaticUpdate();
void Snowflakes_Draw();
void Snowflakes_Create(void* data);
void Snowflakes_StageLoad();
void Snowflakes_EditorDraw();
void Snowflakes_EditorLoad();
void Snowflakes_Serialize();

// Extra Entity Functions


#endif //!OBJ_SNOWFLAKES_H
