#ifndef OBJ_UFO_DECORATION_H
#define OBJ_UFO_DECORATION_H

//Object Class
class UFO_Decoration : Object {

};

//Entity Class
class EntityUFO_Decoration : Entity {

};

//Entity Functions
void UFO_Decoration_Update();
void UFO_Decoration_EarlyUpdate();
void UFO_Decoration_LateUpdate();
void UFO_Decoration_Draw();
void UFO_Decoration_Setup(void* subtype);
void UFO_Decoration_StageLoad();
void UFO_Decoration_GetAttributes();

#endif //!OBJ_UFO_DECORATION_H
