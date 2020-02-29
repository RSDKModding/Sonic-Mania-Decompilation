#ifndef OBJ_MSZCUTSCENEST_H
#define OBJ_MSZCUTSCENEST_H

//Object Class
class MSZCutsceneST : Object {

};

//Entity Class
class EntityMSZCutsceneST : Entity {

};

//Entity Functions
void MSZCutsceneST_Update();
void MSZCutsceneST_EarlyUpdate();
void MSZCutsceneST_LateUpdate();
void MSZCutsceneST_Draw();
void MSZCutsceneST_Setup(void* subtype);
void MSZCutsceneST_StageLoad();
void MSZCutsceneST_GetAttributes();

#endif //!OBJ_MSZCUTSCENEST_H
