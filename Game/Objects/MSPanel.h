#ifndef OBJ_MSPANEL_H
#define OBJ_MSPANEL_H

//Object Class
class MSPanel : Object {

};

//Entity Class
class EntityMSPanel : Entity {

};

//Entity Functions
void MSPanel_Update();
void MSPanel_EarlyUpdate();
void MSPanel_LateUpdate();
void MSPanel_Draw();
void MSPanel_Setup(void* subtype);
void MSPanel_StageLoad();
void MSPanel_GetAttributes();

#endif //!OBJ_MSPANEL_H
