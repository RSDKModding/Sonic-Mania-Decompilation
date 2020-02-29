#ifndef OBJ_BRIDGE_H
#define OBJ_BRIDGE_H

//Object Class
class Bridge : Object {

};

//Entity Class
class EntityBridge : Entity {

};

//Entity Functions
void Bridge_Update();
void Bridge_EarlyUpdate();
void Bridge_LateUpdate();
void Bridge_Draw();
void Bridge_Setup(void* subtype);
void Bridge_StageLoad();
void Bridge_GetAttributes();

#endif //!OBJ_BRIDGE_H
