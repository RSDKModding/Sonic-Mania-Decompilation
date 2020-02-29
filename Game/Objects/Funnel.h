#ifndef OBJ_FUNNEL_H
#define OBJ_FUNNEL_H

//Object Class
class Funnel : Object {

};

//Entity Class
class EntityFunnel : Entity {

};

//Entity Functions
void Funnel_Update();
void Funnel_EarlyUpdate();
void Funnel_LateUpdate();
void Funnel_Draw();
void Funnel_Setup(void* subtype);
void Funnel_StageLoad();
void Funnel_GetAttributes();

#endif //!OBJ_FUNNEL_H
