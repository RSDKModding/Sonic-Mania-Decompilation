#ifndef OBJ_SSZ1INTRO_H
#define OBJ_SSZ1INTRO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSSZ1Intro : Object {

};

// Entity Class
struct EntitySSZ1Intro : Entity {

};

// Object Entity
extern ObjectSSZ1Intro SSZ1Intro;

// Standard Entity Events
void SSZ1Intro_Update();
void SSZ1Intro_LateUpdate();
void SSZ1Intro_StaticUpdate();
void SSZ1Intro_Draw();
void SSZ1Intro_Create(void* data);
void SSZ1Intro_StageLoad();
void SSZ1Intro_EditorDraw();
void SSZ1Intro_EditorLoad();
void SSZ1Intro_Serialize();

// Extra Entity Functions


#endif //!OBJ_SSZ1INTRO_H
