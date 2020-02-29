#ifndef OBJ_TITLESETUP_H
#define OBJ_TITLESETUP_H

//Object Class
class TitleSetup : Object {

};

//Entity Class
class EntityTitleSetup : Entity {

};

//Entity Functions
void TitleSetup_Update();
void TitleSetup_EarlyUpdate();
void TitleSetup_LateUpdate();
void TitleSetup_Draw();
void TitleSetup_Setup(void* subtype);
void TitleSetup_StageLoad();
void TitleSetup_GetAttributes();

#endif //!OBJ_TITLESETUP_H
