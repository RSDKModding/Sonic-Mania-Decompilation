#ifndef OBJ_MSZSETUP_H
#define OBJ_MSZSETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMSZSetup : Object{

};

// Entity Class
struct EntityMSZSetup : Entity {

};

// Object Struct
extern ObjectMSZSetup *MSZSetup;

// Standard Entity Events
void MSZSetup_Update();
void MSZSetup_LateUpdate();
void MSZSetup_StaticUpdate();
void MSZSetup_Draw();
void MSZSetup_Create(void* data);
void MSZSetup_StageLoad();
void MSZSetup_EditorDraw();
void MSZSetup_EditorLoad();
void MSZSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_MSZSETUP_H
