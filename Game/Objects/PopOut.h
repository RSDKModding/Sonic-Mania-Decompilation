#ifndef OBJ_POPOUT_H
#define OBJ_POPOUT_H

//Object Class
class PopOut : Object {

};

//Entity Class
class EntityPopOut : Entity {

};

//Entity Functions
void PopOut_Update();
void PopOut_EarlyUpdate();
void PopOut_LateUpdate();
void PopOut_Draw();
void PopOut_Setup(void* subtype);
void PopOut_StageLoad();
void PopOut_GetAttributes();

#endif //!OBJ_POPOUT_H
