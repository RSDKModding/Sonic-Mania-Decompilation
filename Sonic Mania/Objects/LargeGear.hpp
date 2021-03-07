#ifndef OBJ_LARGEGEAR_H
#define OBJ_LARGEGEAR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLargeGear : Object {

};

// Entity Class
struct EntityLargeGear : Entity {

};

// Object Struct
extern ObjectLargeGear *LargeGear;

// Standard Entity Events
void LargeGear_Update();
void LargeGear_LateUpdate();
void LargeGear_StaticUpdate();
void LargeGear_Draw();
void LargeGear_Create(void* data);
void LargeGear_StageLoad();
void LargeGear_EditorDraw();
void LargeGear_EditorLoad();
void LargeGear_Serialize();

// Extra Entity Functions


#endif //!OBJ_LARGEGEAR_H
