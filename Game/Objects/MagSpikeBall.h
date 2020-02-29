#ifndef OBJ_MAGSPIKEBALL_H
#define OBJ_MAGSPIKEBALL_H

//Object Class
class MagSpikeBall : Object {

};

//Entity Class
class EntityMagSpikeBall : Entity {

};

//Entity Functions
void MagSpikeBall_Update();
void MagSpikeBall_EarlyUpdate();
void MagSpikeBall_LateUpdate();
void MagSpikeBall_Draw();
void MagSpikeBall_Setup(void* subtype);
void MagSpikeBall_StageLoad();
void MagSpikeBall_GetAttributes();

#endif //!OBJ_MAGSPIKEBALL_H
