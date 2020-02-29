#ifndef OBJ_TITLELOGO_H
#define OBJ_TITLELOGO_H

//Object Class
class TitleLogo : Object {

};

//Entity Class
class EntityTitleLogo : Entity {

};

//Entity Functions
void TitleLogo_Update();
void TitleLogo_EarlyUpdate();
void TitleLogo_LateUpdate();
void TitleLogo_Draw();
void TitleLogo_Setup(void* subtype);
void TitleLogo_StageLoad();
void TitleLogo_GetAttributes();

#endif //!OBJ_TITLELOGO_H
