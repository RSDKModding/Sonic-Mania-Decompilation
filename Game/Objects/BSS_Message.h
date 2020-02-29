#ifndef OBJ_BSS_MESSAGE_H
#define OBJ_BSS_MESSAGE_H

//Object Class
class BSS_Message : Object {

};

//Entity Class
class EntityBSS_Message : Entity {

};

//Entity Functions
void BSS_Message_Update();
void BSS_Message_EarlyUpdate();
void BSS_Message_LateUpdate();
void BSS_Message_Draw();
void BSS_Message_Setup(void* subtype);
void BSS_Message_StageLoad();
void BSS_Message_GetAttributes();

#endif //!OBJ_BSS_MESSAGE_H
