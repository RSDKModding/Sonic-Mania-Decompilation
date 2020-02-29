#ifndef OBJ_UFO_HUD_H
#define OBJ_UFO_HUD_H

//Object Class
class UFO_HUD : Object {

};

//Entity Class
class EntityUFO_HUD : Entity {

};

//Entity Functions
void UFO_HUD_Update();
void UFO_HUD_EarlyUpdate();
void UFO_HUD_LateUpdate();
void UFO_HUD_Draw();
void UFO_HUD_Setup(void* subtype);
void UFO_HUD_StageLoad();
void UFO_HUD_GetAttributes();

#endif //!OBJ_UFO_HUD_H
