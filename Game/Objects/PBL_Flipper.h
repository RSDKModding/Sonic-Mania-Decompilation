#ifndef OBJ_PBL_FLIPPER_H
#define OBJ_PBL_FLIPPER_H

//Object Class
class PBL_Flipper : Object {

};

//Entity Class
class EntityPBL_Flipper : Entity {

};

//Entity Functions
void PBL_Flipper_Update();
void PBL_Flipper_EarlyUpdate();
void PBL_Flipper_LateUpdate();
void PBL_Flipper_Draw();
void PBL_Flipper_Setup(void* subtype);
void PBL_Flipper_StageLoad();
void PBL_Flipper_GetAttributes();

#endif //!OBJ_PBL_FLIPPER_H
