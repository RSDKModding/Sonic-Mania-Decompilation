#ifndef OBJ_SHIELD_H
#define OBJ_SHIELD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectShield : Object{

};

// Entity Class
struct EntityShield : Entity {

};

// Object Struct
extern ObjectShield *Shield;

// Standard Entity Events
void Shield_Update();
void Shield_LateUpdate();
void Shield_StaticUpdate();
void Shield_Draw();
void Shield_Create(void* data);
void Shield_StageLoad();
void Shield_EditorDraw();
void Shield_EditorLoad();
void Shield_Serialize();

// Extra Entity Functions


#endif //!OBJ_SHIELD_H
