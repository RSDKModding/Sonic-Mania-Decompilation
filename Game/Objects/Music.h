#ifndef OBJ_MUSIC_H
#define OBJ_MUSIC_H

#include "../../SonicMania.h"

//Object Class
class ObjectMusic : public Object {
public:

};

//Entity Class
class EntityMusic : public Entity {
public:

};

//Object Entity
ObjectMusic Music;

//Entity Functions
void Music_Update();
void Music_EarlyUpdate();
void Music_LateUpdate();
void Music_Draw();
void Music_Setup(void* subtype);
void Music_StageLoad();
void Music_GetAttributes();

#endif //!OBJ_MUSIC_H
