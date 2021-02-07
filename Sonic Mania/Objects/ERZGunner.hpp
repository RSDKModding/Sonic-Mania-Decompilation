#ifndef OBJ_ERZGUNNER_H
#define OBJ_ERZGUNNER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectERZGunner : Object{

};

// Entity Class
struct EntityERZGunner : Entity {

};

// Object Struct
extern ObjectERZGunner *ERZGunner;

// Standard Entity Events
void ERZGunner_Update();
void ERZGunner_LateUpdate();
void ERZGunner_StaticUpdate();
void ERZGunner_Draw();
void ERZGunner_Create(void* data);
void ERZGunner_StageLoad();
void ERZGunner_EditorDraw();
void ERZGunner_EditorLoad();
void ERZGunner_Serialize();

// Extra Entity Functions


#endif //!OBJ_ERZGUNNER_H
