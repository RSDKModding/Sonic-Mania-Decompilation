#ifndef OBJ_PSZ2INTRO_H
#define OBJ_PSZ2INTRO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPSZ2Intro : Object {

};

// Entity Class
struct EntityPSZ2Intro : Entity {

};

// Object Struct
extern ObjectPSZ2Intro PSZ2Intro;

// Standard Entity Events
void PSZ2Intro_Update();
void PSZ2Intro_LateUpdate();
void PSZ2Intro_StaticUpdate();
void PSZ2Intro_Draw();
void PSZ2Intro_Create(void* data);
void PSZ2Intro_StageLoad();
void PSZ2Intro_EditorDraw();
void PSZ2Intro_EditorLoad();
void PSZ2Intro_Serialize();

// Extra Entity Functions


#endif //!OBJ_PSZ2INTRO_H
