#ifndef OBJ_HCZ1INTRO_H
#define OBJ_HCZ1INTRO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHCZ1Intro : Object {

};

// Entity Class
struct EntityHCZ1Intro : Entity {

};

// Object Entity
extern ObjectHCZ1Intro HCZ1Intro;

// Standard Entity Events
void HCZ1Intro_Update();
void HCZ1Intro_LateUpdate();
void HCZ1Intro_StaticUpdate();
void HCZ1Intro_Draw();
void HCZ1Intro_Create(void* data);
void HCZ1Intro_StageLoad();
void HCZ1Intro_EditorDraw();
void HCZ1Intro_EditorLoad();
void HCZ1Intro_Serialize();

// Extra Entity Functions


#endif //!OBJ_HCZ1INTRO_H
