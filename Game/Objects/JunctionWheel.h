#ifndef OBJ_JUNCTIONWHEEL_H
#define OBJ_JUNCTIONWHEEL_H

//Object Class
class JunctionWheel : Object {

};

//Entity Class
class EntityJunctionWheel : Entity {

};

//Entity Functions
void JunctionWheel_Update();
void JunctionWheel_EarlyUpdate();
void JunctionWheel_LateUpdate();
void JunctionWheel_Draw();
void JunctionWheel_Setup(void* subtype);
void JunctionWheel_StageLoad();
void JunctionWheel_GetAttributes();

#endif //!OBJ_JUNCTIONWHEEL_H
