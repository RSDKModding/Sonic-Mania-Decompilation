#ifndef OBJ_TITLESETUP_H
#define OBJ_TITLESETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTitleSetup : Object {

};

// Entity Class
struct EntityTitleSetup : Entity {

};

// Object Struct
extern ObjectTitleSetup *TitleSetup;

// Standard Entity Events
void TitleSetup_Update();
void TitleSetup_LateUpdate();
void TitleSetup_StaticUpdate();
void TitleSetup_Draw();
void TitleSetup_Create(void* data);
void TitleSetup_StageLoad();
void TitleSetup_EditorDraw();
void TitleSetup_EditorLoad();
void TitleSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_TITLESETUP_H
