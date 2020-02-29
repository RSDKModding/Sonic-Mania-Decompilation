#ifndef OBJ_BSS_COLLECTED_H
#define OBJ_BSS_COLLECTED_H

//Object Class
class BSS_Collected : Object {

};

//Entity Class
class EntityBSS_Collected : Entity {

};

//Entity Functions
void BSS_Collected_Update();
void BSS_Collected_EarlyUpdate();
void BSS_Collected_LateUpdate();
void BSS_Collected_Draw();
void BSS_Collected_Setup(void* subtype);
void BSS_Collected_StageLoad();
void BSS_Collected_GetAttributes();

#endif //!OBJ_BSS_COLLECTED_H
