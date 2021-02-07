#ifndef OBJ_MUSIC_H
#define OBJ_MUSIC_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMusic : Object{

};

// Entity Class
struct EntityMusic : Entity {

};

// Object Struct
extern ObjectMusic *Music;

// Standard Entity Events
void Music_Update();
void Music_LateUpdate();
void Music_StaticUpdate();
void Music_Draw();
void Music_Create(void* data);
void Music_StageLoad();
void Music_EditorDraw();
void Music_EditorLoad();
void Music_Serialize();

// Extra Entity Functions


#endif //!OBJ_MUSIC_H
