#ifndef OBJ_TITLESONIC_H
#define OBJ_TITLESONIC_H

//Object Class
class TitleSonic : Object {

};

//Entity Class
class EntityTitleSonic : Entity {

};

//Entity Functions
void TitleSonic_Update();
void TitleSonic_EarlyUpdate();
void TitleSonic_LateUpdate();
void TitleSonic_Draw();
void TitleSonic_Setup(void* subtype);
void TitleSonic_StageLoad();
void TitleSonic_GetAttributes();

#endif //!OBJ_TITLESONIC_H
