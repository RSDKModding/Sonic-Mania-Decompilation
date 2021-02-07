#ifndef OBJ_GRABBER_H
#define OBJ_GRABBER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectGrabber : Object{

};

// Entity Class
struct EntityGrabber : Entity {

};

// Object Struct
extern ObjectGrabber *Grabber;

// Standard Entity Events
void Grabber_Update();
void Grabber_LateUpdate();
void Grabber_StaticUpdate();
void Grabber_Draw();
void Grabber_Create(void* data);
void Grabber_StageLoad();
void Grabber_EditorDraw();
void Grabber_EditorLoad();
void Grabber_Serialize();

// Extra Entity Functions


#endif //!OBJ_GRABBER_H
