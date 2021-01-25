#ifndef OBJ_PLAYERPROBE_H
#define OBJ_PLAYERPROBE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPlayerProbe : Object {

};

// Entity Class
struct EntityPlayerProbe : Entity {

};

// Object Entity
extern ObjectPlayerProbe PlayerProbe;

// Standard Entity Events
void PlayerProbe_Update();
void PlayerProbe_LateUpdate();
void PlayerProbe_StaticUpdate();
void PlayerProbe_Draw();
void PlayerProbe_Create(void* data);
void PlayerProbe_StageLoad();
void PlayerProbe_EditorDraw();
void PlayerProbe_EditorLoad();
void PlayerProbe_Serialize();

// Extra Entity Functions


#endif //!OBJ_PLAYERPROBE_H
