#ifndef OBJ_PBL_HUD_H
#define OBJ_PBL_HUD_H

//Object Class
class PBL_HUD : Object {

};

//Entity Class
class EntityPBL_HUD : Entity {

};

//Entity Functions
void PBL_HUD_Update();
void PBL_HUD_EarlyUpdate();
void PBL_HUD_LateUpdate();
void PBL_HUD_Draw();
void PBL_HUD_Setup(void* subtype);
void PBL_HUD_StageLoad();
void PBL_HUD_GetAttributes();

#endif //!OBJ_PBL_HUD_H
