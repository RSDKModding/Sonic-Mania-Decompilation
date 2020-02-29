#ifndef OBJ_PLAYERPROBE_H
#define OBJ_PLAYERPROBE_H

#include "../../SonicMania.h"

//Object Class
class ObjectPlayerProbe : public Object {
public:

};

//Entity Class
class EntityPlayerProbe : public Entity {
public:

};

//Object Entity
ObjectPlayerProbe PlayerProbe;

//Entity Functions
void PlayerProbe_Update();
void PlayerProbe_EarlyUpdate();
void PlayerProbe_LateUpdate();
void PlayerProbe_Draw();
void PlayerProbe_Setup(void* subtype);
void PlayerProbe_StageLoad();
void PlayerProbe_GetAttributes();

#endif //!OBJ_PLAYERPROBE_H
