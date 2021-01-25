#ifndef OBJ_FBZMISSILE_H
#define OBJ_FBZMISSILE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFBZMissile : Object {

};

// Entity Class
struct EntityFBZMissile : Entity {

};

// Object Entity
extern ObjectFBZMissile FBZMissile;

// Standard Entity Events
void FBZMissile_Update();
void FBZMissile_LateUpdate();
void FBZMissile_StaticUpdate();
void FBZMissile_Draw();
void FBZMissile_Create(void* data);
void FBZMissile_StageLoad();
void FBZMissile_EditorDraw();
void FBZMissile_EditorLoad();
void FBZMissile_Serialize();

// Extra Entity Functions


#endif //!OBJ_FBZMISSILE_H
