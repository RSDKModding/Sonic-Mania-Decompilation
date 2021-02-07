#ifndef OBJ_FBZSTORM_H
#define OBJ_FBZSTORM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFBZStorm : Object{

};

// Entity Class
struct EntityFBZStorm : Entity {

};

// Object Struct
extern ObjectFBZStorm *FBZStorm;

// Standard Entity Events
void FBZStorm_Update();
void FBZStorm_LateUpdate();
void FBZStorm_StaticUpdate();
void FBZStorm_Draw();
void FBZStorm_Create(void* data);
void FBZStorm_StageLoad();
void FBZStorm_EditorDraw();
void FBZStorm_EditorLoad();
void FBZStorm_Serialize();

// Extra Entity Functions


#endif //!OBJ_FBZSTORM_H
