#ifndef OBJ_AIZSETUP_H
#define OBJ_AIZSETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectAIZSetup : Object {

};

// Entity Class
struct EntityAIZSetup : Entity {

};

// Object Struct
extern ObjectAIZSetup *AIZSetup;

// Standard Entity Events
void AIZSetup_Update();
void AIZSetup_LateUpdate();
void AIZSetup_StaticUpdate();
void AIZSetup_Draw();
void AIZSetup_Create(void* data);
void AIZSetup_StageLoad();
void AIZSetup_EditorDraw();
void AIZSetup_EditorLoad();
void AIZSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_AIZSETUP_H
