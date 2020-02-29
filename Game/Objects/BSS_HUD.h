#ifndef OBJ_BSS_HUD_H
#define OBJ_BSS_HUD_H

//Object Class
class BSS_HUD : Object {

};

//Entity Class
class EntityBSS_HUD : Entity {

};

//Entity Functions
void BSS_HUD_Update();
void BSS_HUD_EarlyUpdate();
void BSS_HUD_LateUpdate();
void BSS_HUD_Draw();
void BSS_HUD_Setup(void* subtype);
void BSS_HUD_StageLoad();
void BSS_HUD_GetAttributes();

#endif //!OBJ_BSS_HUD_H
