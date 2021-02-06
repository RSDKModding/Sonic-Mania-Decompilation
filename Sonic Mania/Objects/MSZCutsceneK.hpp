#ifndef OBJ_MSZCUTSCENEK_H
#define OBJ_MSZCUTSCENEK_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMSZCutsceneK : Object {

};

// Entity Class
struct EntityMSZCutsceneK : Entity {

};

// Object Entity
extern ObjectMSZCutsceneK MSZCutsceneK;

// Standard Entity Events
void MSZCutsceneK_Update();
void MSZCutsceneK_LateUpdate();
void MSZCutsceneK_StaticUpdate();
void MSZCutsceneK_Draw();
void MSZCutsceneK_Create(void* data);
void MSZCutsceneK_StageLoad();
void MSZCutsceneK_EditorDraw();
void MSZCutsceneK_EditorLoad();
void MSZCutsceneK_Serialize();

// Extra Entity Functions


#endif //!OBJ_MSZCUTSCENEK_H
