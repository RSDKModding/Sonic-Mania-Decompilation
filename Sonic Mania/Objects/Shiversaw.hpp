#ifndef OBJ_SHIVERSAW_H
#define OBJ_SHIVERSAW_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectShiversaw : Object{

};

// Entity Class
struct EntityShiversaw : Entity {

};

// Object Struct
extern ObjectShiversaw *Shiversaw;

// Standard Entity Events
void Shiversaw_Update();
void Shiversaw_LateUpdate();
void Shiversaw_StaticUpdate();
void Shiversaw_Draw();
void Shiversaw_Create(void* data);
void Shiversaw_StageLoad();
void Shiversaw_EditorDraw();
void Shiversaw_EditorLoad();
void Shiversaw_Serialize();

// Extra Entity Functions


#endif //!OBJ_SHIVERSAW_H
