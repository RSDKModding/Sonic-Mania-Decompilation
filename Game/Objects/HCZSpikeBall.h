#ifndef OBJ_HCZSPIKEBALL_H
#define OBJ_HCZSPIKEBALL_H

//Object Class
class HCZSpikeBall : Object {

};

//Entity Class
class EntityHCZSpikeBall : Entity {

};

//Entity Functions
void HCZSpikeBall_Update();
void HCZSpikeBall_EarlyUpdate();
void HCZSpikeBall_LateUpdate();
void HCZSpikeBall_Draw();
void HCZSpikeBall_Setup(void* subtype);
void HCZSpikeBall_StageLoad();
void HCZSpikeBall_GetAttributes();

#endif //!OBJ_HCZSPIKEBALL_H
