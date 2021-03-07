#ifndef OBJ_PLATFORMNODE_H
#define OBJ_PLATFORMNODE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPlatformNode : Object {

};

// Entity Class
struct EntityPlatformNode : Entity {

};

// Object Struct
extern ObjectPlatformNode *PlatformNode;

// Standard Entity Events
void PlatformNode_Update();
void PlatformNode_LateUpdate();
void PlatformNode_StaticUpdate();
void PlatformNode_Draw();
void PlatformNode_Create(void* data);
void PlatformNode_StageLoad();
void PlatformNode_EditorDraw();
void PlatformNode_EditorLoad();
void PlatformNode_Serialize();

// Extra Entity Functions


#endif //!OBJ_PLATFORMNODE_H
