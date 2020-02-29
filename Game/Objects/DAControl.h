#ifndef OBJ_DACONTROL_H
#define OBJ_DACONTROL_H

//Object Class
class DAControl : Object {

};

//Entity Class
class EntityDAControl : Entity {

};

//Entity Functions
void DAControl_Update();
void DAControl_EarlyUpdate();
void DAControl_LateUpdate();
void DAControl_Draw();
void DAControl_Setup(void* subtype);
void DAControl_StageLoad();
void DAControl_GetAttributes();

#endif //!OBJ_DACONTROL_H
