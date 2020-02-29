#ifndef OBJ_HANGGLIDER_H
#define OBJ_HANGGLIDER_H

#include "../../SonicMania.h"

//Object Class
class ObjectHangGlider : public Object {
public:

};

//Entity Class
class EntityHangGlider : public Entity {
public:

};

//Object Entity
ObjectHangGlider HangGlider;

//Entity Functions
void HangGlider_Update();
void HangGlider_EarlyUpdate();
void HangGlider_LateUpdate();
void HangGlider_Draw();
void HangGlider_Setup(void* subtype);
void HangGlider_StageLoad();
void HangGlider_GetAttributes();

#endif //!OBJ_HANGGLIDER_H
