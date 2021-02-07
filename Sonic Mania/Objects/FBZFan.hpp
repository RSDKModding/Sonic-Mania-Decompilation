#ifndef OBJ_FBZFAN_H
#define OBJ_FBZFAN_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFBZFan : Object{

};

// Entity Class
struct EntityFBZFan : Entity {

};

// Object Struct
extern ObjectFBZFan *FBZFan;

// Standard Entity Events
void FBZFan_Update();
void FBZFan_LateUpdate();
void FBZFan_StaticUpdate();
void FBZFan_Draw();
void FBZFan_Create(void* data);
void FBZFan_StageLoad();
void FBZFan_EditorDraw();
void FBZFan_EditorLoad();
void FBZFan_Serialize();

// Extra Entity Functions


#endif //!OBJ_FBZFAN_H
