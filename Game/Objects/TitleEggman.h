#ifndef OBJ_TITLEEGGMAN_H
#define OBJ_TITLEEGGMAN_H

//Object Class
class TitleEggman : Object {

};

//Entity Class
class EntityTitleEggman : Entity {

};

//Entity Functions
void TitleEggman_Update();
void TitleEggman_EarlyUpdate();
void TitleEggman_LateUpdate();
void TitleEggman_Draw();
void TitleEggman_Setup(void* subtype);
void TitleEggman_StageLoad();
void TitleEggman_GetAttributes();

#endif //!OBJ_TITLEEGGMAN_H
