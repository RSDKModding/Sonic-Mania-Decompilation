#ifndef OBJ_PSZLAUNCHER_H
#define OBJ_PSZLAUNCHER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPSZLauncher : Object {

};

// Entity Class
struct EntityPSZLauncher : Entity {

};

// Object Entity
extern ObjectPSZLauncher PSZLauncher;

// Standard Entity Events
void PSZLauncher_Update();
void PSZLauncher_LateUpdate();
void PSZLauncher_StaticUpdate();
void PSZLauncher_Draw();
void PSZLauncher_Create(void* data);
void PSZLauncher_StageLoad();
void PSZLauncher_EditorDraw();
void PSZLauncher_EditorLoad();
void PSZLauncher_Serialize();

// Extra Entity Functions


#endif //!OBJ_PSZLAUNCHER_H
