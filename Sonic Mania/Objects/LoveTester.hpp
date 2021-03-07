#ifndef OBJ_LOVETESTER_H
#define OBJ_LOVETESTER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLoveTester : Object {

};

// Entity Class
struct EntityLoveTester : Entity {

};

// Object Struct
extern ObjectLoveTester *LoveTester;

// Standard Entity Events
void LoveTester_Update();
void LoveTester_LateUpdate();
void LoveTester_StaticUpdate();
void LoveTester_Draw();
void LoveTester_Create(void* data);
void LoveTester_StageLoad();
void LoveTester_EditorDraw();
void LoveTester_EditorLoad();
void LoveTester_Serialize();

// Extra Entity Functions


#endif //!OBJ_LOVETESTER_H
