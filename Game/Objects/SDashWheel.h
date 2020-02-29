#ifndef OBJ_SDASHWHEEL_H
#define OBJ_SDASHWHEEL_H

//Object Class
class SDashWheel : Object {

};

//Entity Class
class EntitySDashWheel : Entity {

};

//Entity Functions
void SDashWheel_Update();
void SDashWheel_EarlyUpdate();
void SDashWheel_LateUpdate();
void SDashWheel_Draw();
void SDashWheel_Setup(void* subtype);
void SDashWheel_StageLoad();
void SDashWheel_GetAttributes();

#endif //!OBJ_SDASHWHEEL_H
