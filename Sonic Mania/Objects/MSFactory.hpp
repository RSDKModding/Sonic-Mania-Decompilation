#ifndef OBJ_MSFACTORY_H
#define OBJ_MSFACTORY_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMSFactory : Object {

};

// Entity Class
struct EntityMSFactory : Entity {

};

// Object Entity
extern ObjectMSFactory MSFactory;

// Standard Entity Events
void MSFactory_Update();
void MSFactory_LateUpdate();
void MSFactory_StaticUpdate();
void MSFactory_Draw();
void MSFactory_Create(void* data);
void MSFactory_StageLoad();
void MSFactory_EditorDraw();
void MSFactory_EditorLoad();
void MSFactory_Serialize();

// Extra Entity Functions


#endif //!OBJ_MSFACTORY_H
