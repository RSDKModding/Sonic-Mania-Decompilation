#ifndef OBJ_HCZSETUP_H
#define OBJ_HCZSETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHCZSetup : Object{

};

// Entity Class
struct EntityHCZSetup : Entity {

};

// Object Struct
extern ObjectHCZSetup *HCZSetup;

// Standard Entity Events
void HCZSetup_Update();
void HCZSetup_LateUpdate();
void HCZSetup_StaticUpdate();
void HCZSetup_Draw();
void HCZSetup_Create(void* data);
void HCZSetup_StageLoad();
void HCZSetup_EditorDraw();
void HCZSetup_EditorLoad();
void HCZSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_HCZSETUP_H
