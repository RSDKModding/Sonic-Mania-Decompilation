#ifndef OBJ_SSZSPIKEBALL_H
#define OBJ_SSZSPIKEBALL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSSZSpikeBall : Object {

};

// Entity Class
struct EntitySSZSpikeBall : Entity {

};

// Object Struct
extern ObjectSSZSpikeBall *SSZSpikeBall;

// Standard Entity Events
void SSZSpikeBall_Update();
void SSZSpikeBall_LateUpdate();
void SSZSpikeBall_StaticUpdate();
void SSZSpikeBall_Draw();
void SSZSpikeBall_Create(void* data);
void SSZSpikeBall_StageLoad();
void SSZSpikeBall_EditorDraw();
void SSZSpikeBall_EditorLoad();
void SSZSpikeBall_Serialize();

// Extra Entity Functions


#endif //!OBJ_SSZSPIKEBALL_H
