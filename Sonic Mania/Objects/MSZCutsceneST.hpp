#ifndef OBJ_MSZCUTSCENEST_H
#define OBJ_MSZCUTSCENEST_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMSZCutsceneST : Object {

};

// Entity Class
struct EntityMSZCutsceneST : Entity {

};

// Object Entity
extern ObjectMSZCutsceneST MSZCutsceneST;

// Standard Entity Events
void MSZCutsceneST_Update();
void MSZCutsceneST_LateUpdate();
void MSZCutsceneST_StaticUpdate();
void MSZCutsceneST_Draw();
void MSZCutsceneST_Create(void* data);
void MSZCutsceneST_StageLoad();
void MSZCutsceneST_EditorDraw();
void MSZCutsceneST_EditorLoad();
void MSZCutsceneST_Serialize();

// Extra Entity Functions


#endif //!OBJ_MSZCUTSCENEST_H
