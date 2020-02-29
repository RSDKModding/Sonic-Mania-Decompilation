#ifndef OBJ_BSS_MESSAGE_H
#define OBJ_BSS_MESSAGE_H

#include "../../SonicMania.h"

//Object Class
class ObjectBSS_Message : public Object {
public:

};

//Entity Class
class EntityBSS_Message : public Entity {
public:

};

//Object Entity
ObjectBSS_Message BSS_Message;

//Entity Functions
void BSS_Message_Update();
void BSS_Message_EarlyUpdate();
void BSS_Message_LateUpdate();
void BSS_Message_Draw();
void BSS_Message_Setup(void* subtype);
void BSS_Message_StageLoad();
void BSS_Message_GetAttributes();

#endif //!OBJ_BSS_MESSAGE_H
