#ifndef OBJ_PLATFORMNODE_H
#define OBJ_PLATFORMNODE_H

//Object Class
class PlatformNode : Object {

};

//Entity Class
class EntityPlatformNode : Entity {

};

//Entity Functions
void PlatformNode_Update();
void PlatformNode_EarlyUpdate();
void PlatformNode_LateUpdate();
void PlatformNode_Draw();
void PlatformNode_Setup(void* subtype);
void PlatformNode_StageLoad();
void PlatformNode_GetAttributes();

#endif //!OBJ_PLATFORMNODE_H
