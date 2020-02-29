#ifndef OBJ_MICDROP_H
#define OBJ_MICDROP_H

//Object Class
class MicDrop : Object {

};

//Entity Class
class EntityMicDrop : Entity {

};

//Entity Functions
void MicDrop_Update();
void MicDrop_EarlyUpdate();
void MicDrop_LateUpdate();
void MicDrop_Draw();
void MicDrop_Setup(void* subtype);
void MicDrop_StageLoad();
void MicDrop_GetAttributes();

#endif //!OBJ_MICDROP_H
