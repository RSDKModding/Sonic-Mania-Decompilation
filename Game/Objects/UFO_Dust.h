#ifndef OBJ_UFO_DUST_H
#define OBJ_UFO_DUST_H

//Object Class
class UFO_Dust : Object {

};

//Entity Class
class EntityUFO_Dust : Entity {

};

//Entity Functions
void UFO_Dust_Update();
void UFO_Dust_EarlyUpdate();
void UFO_Dust_LateUpdate();
void UFO_Dust_Draw();
void UFO_Dust_Setup(void* subtype);
void UFO_Dust_StageLoad();
void UFO_Dust_GetAttributes();

#endif //!OBJ_UFO_DUST_H
