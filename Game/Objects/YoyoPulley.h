#ifndef OBJ_YOYOPULLEY_H
#define OBJ_YOYOPULLEY_H

//Object Class
class YoyoPulley : Object {

};

//Entity Class
class EntityYoyoPulley : Entity {

};

//Entity Functions
void YoyoPulley_Update();
void YoyoPulley_EarlyUpdate();
void YoyoPulley_LateUpdate();
void YoyoPulley_Draw();
void YoyoPulley_Setup(void* subtype);
void YoyoPulley_StageLoad();
void YoyoPulley_GetAttributes();

#endif //!OBJ_YOYOPULLEY_H
