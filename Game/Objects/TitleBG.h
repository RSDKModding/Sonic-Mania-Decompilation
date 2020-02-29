#ifndef OBJ_TITLEBG_H
#define OBJ_TITLEBG_H

//Object Class
class TitleBG : Object {

};

//Entity Class
class EntityTitleBG : Entity {

};

//Entity Functions
void TitleBG_Update();
void TitleBG_EarlyUpdate();
void TitleBG_LateUpdate();
void TitleBG_Draw();
void TitleBG_Setup(void* subtype);
void TitleBG_StageLoad();
void TitleBG_GetAttributes();

#endif //!OBJ_TITLEBG_H
