#ifndef OBJ_HILOSIGN_H
#define OBJ_HILOSIGN_H

//Object Class
class HiLoSign : Object {

};

//Entity Class
class EntityHiLoSign : Entity {

};

//Entity Functions
void HiLoSign_Update();
void HiLoSign_EarlyUpdate();
void HiLoSign_LateUpdate();
void HiLoSign_Draw();
void HiLoSign_Setup(void* subtype);
void HiLoSign_StageLoad();
void HiLoSign_GetAttributes();

#endif //!OBJ_HILOSIGN_H
