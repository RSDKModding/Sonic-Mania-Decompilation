#ifndef OBJ_PULLCHAIN_H
#define OBJ_PULLCHAIN_H

//Object Class
class PullChain : Object {

};

//Entity Class
class EntityPullChain : Entity {

};

//Entity Functions
void PullChain_Update();
void PullChain_EarlyUpdate();
void PullChain_LateUpdate();
void PullChain_Draw();
void PullChain_Setup(void* subtype);
void PullChain_StageLoad();
void PullChain_GetAttributes();

#endif //!OBJ_PULLCHAIN_H
