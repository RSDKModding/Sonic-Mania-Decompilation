#ifndef OBJ_FROSTTHROWER_H
#define OBJ_FROSTTHROWER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFrostThrower : Object {

};

// Entity Class
struct EntityFrostThrower : Entity {

};

// Object Entity
extern ObjectFrostThrower FrostThrower;

// Standard Entity Events
void FrostThrower_Update();
void FrostThrower_LateUpdate();
void FrostThrower_StaticUpdate();
void FrostThrower_Draw();
void FrostThrower_Create(void* data);
void FrostThrower_StageLoad();
void FrostThrower_EditorDraw();
void FrostThrower_EditorLoad();
void FrostThrower_Serialize();

// Extra Entity Functions


#endif //!OBJ_FROSTTHROWER_H
