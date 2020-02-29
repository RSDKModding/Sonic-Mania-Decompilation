#ifndef OBJ_CPZSHUTTER_H
#define OBJ_CPZSHUTTER_H

//Object Class
class CPZShutter : Object {

};

//Entity Class
class EntityCPZShutter : Entity {

};

//Entity Functions
void CPZShutter_Update();
void CPZShutter_EarlyUpdate();
void CPZShutter_LateUpdate();
void CPZShutter_Draw();
void CPZShutter_Setup(void* subtype);
void CPZShutter_StageLoad();
void CPZShutter_GetAttributes();

#endif //!OBJ_CPZSHUTTER_H
