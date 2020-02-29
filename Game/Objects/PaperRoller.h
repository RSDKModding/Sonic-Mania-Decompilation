#ifndef OBJ_PAPERROLLER_H
#define OBJ_PAPERROLLER_H

//Object Class
class PaperRoller : Object {

};

//Entity Class
class EntityPaperRoller : Entity {

};

//Entity Functions
void PaperRoller_Update();
void PaperRoller_EarlyUpdate();
void PaperRoller_LateUpdate();
void PaperRoller_Draw();
void PaperRoller_Setup(void* subtype);
void PaperRoller_StageLoad();
void PaperRoller_GetAttributes();

#endif //!OBJ_PAPERROLLER_H
