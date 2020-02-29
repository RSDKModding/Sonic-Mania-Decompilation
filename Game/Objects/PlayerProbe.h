#ifndef OBJ_PLAYERPROBE_H
#define OBJ_PLAYERPROBE_H

//Object Class
class PlayerProbe : Object {

};

//Entity Class
class EntityPlayerProbe : Entity {

};

//Entity Functions
void PlayerProbe_Update();
void PlayerProbe_EarlyUpdate();
void PlayerProbe_LateUpdate();
void PlayerProbe_Draw();
void PlayerProbe_Setup(void* subtype);
void PlayerProbe_StageLoad();
void PlayerProbe_GetAttributes();

#endif //!OBJ_PLAYERPROBE_H
