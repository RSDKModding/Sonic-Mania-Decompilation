#ifndef OBJ_CONTINUESETUP_H
#define OBJ_CONTINUESETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectContinueSetup : Object {

};

// Entity Class
struct EntityContinueSetup : Entity {

};

// Object Struct
extern ObjectContinueSetup *ContinueSetup;

// Standard Entity Events
void ContinueSetup_Update();
void ContinueSetup_LateUpdate();
void ContinueSetup_StaticUpdate();
void ContinueSetup_Draw();
void ContinueSetup_Create(void* data);
void ContinueSetup_StageLoad();
void ContinueSetup_EditorDraw();
void ContinueSetup_EditorLoad();
void ContinueSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_CONTINUESETUP_H
