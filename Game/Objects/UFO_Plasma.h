#ifndef OBJ_UFO_PLASMA_H
#define OBJ_UFO_PLASMA_H

//Object Class
class UFO_Plasma : Object {

};

//Entity Class
class EntityUFO_Plasma : Entity {

};

//Entity Functions
void UFO_Plasma_Update();
void UFO_Plasma_EarlyUpdate();
void UFO_Plasma_LateUpdate();
void UFO_Plasma_Draw();
void UFO_Plasma_Setup(void* subtype);
void UFO_Plasma_StageLoad();
void UFO_Plasma_GetAttributes();

#endif //!OBJ_UFO_PLASMA_H
