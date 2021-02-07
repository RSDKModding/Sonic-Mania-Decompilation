#ifndef OBJ_SOUNDBOARD_H
#define OBJ_SOUNDBOARD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSoundboard : Object{

};

// Entity Class
struct EntitySoundboard : Entity {

};

// Object Struct
extern ObjectSoundboard *Soundboard;

// Standard Entity Events
void Soundboard_Update();
void Soundboard_LateUpdate();
void Soundboard_StaticUpdate();
void Soundboard_Draw();
void Soundboard_Create(void* data);
void Soundboard_StageLoad();
void Soundboard_EditorDraw();
void Soundboard_EditorLoad();
void Soundboard_Serialize();

// Extra Entity Functions


#endif //!OBJ_SOUNDBOARD_H
