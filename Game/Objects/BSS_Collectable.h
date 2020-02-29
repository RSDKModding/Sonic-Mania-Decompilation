#ifndef OBJ_BSS_COLLECTABLE_H
#define OBJ_BSS_COLLECTABLE_H

//Object Class
class BSS_Collectable : Object {

};

//Entity Class
class EntityBSS_Collectable : Entity {

};

//Entity Functions
void BSS_Collectable_Update();
void BSS_Collectable_EarlyUpdate();
void BSS_Collectable_LateUpdate();
void BSS_Collectable_Draw();
void BSS_Collectable_Setup(void* subtype);
void BSS_Collectable_StageLoad();
void BSS_Collectable_GetAttributes();

#endif //!OBJ_BSS_COLLECTABLE_H
