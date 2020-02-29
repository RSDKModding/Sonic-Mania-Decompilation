#ifndef OBJ_MUSIC_H
#define OBJ_MUSIC_H

//Object Class
class Music : Object {

};

//Entity Class
class EntityMusic : Entity {

};

//Entity Functions
void Music_Update();
void Music_EarlyUpdate();
void Music_LateUpdate();
void Music_Draw();
void Music_Setup(void* subtype);
void Music_StageLoad();
void Music_GetAttributes();

#endif //!OBJ_MUSIC_H
