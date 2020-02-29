#ifndef OBJ_SSZSPIKEBALL_H
#define OBJ_SSZSPIKEBALL_H

//Object Class
class SSZSpikeBall : Object {

};

//Entity Class
class EntitySSZSpikeBall : Entity {

};

//Entity Functions
void SSZSpikeBall_Update();
void SSZSpikeBall_EarlyUpdate();
void SSZSpikeBall_LateUpdate();
void SSZSpikeBall_Draw();
void SSZSpikeBall_Setup(void* subtype);
void SSZSpikeBall_StageLoad();
void SSZSpikeBall_GetAttributes();

#endif //!OBJ_SSZSPIKEBALL_H
