#ifndef OBJ_LOCALIZATION_H
#define OBJ_LOCALIZATION_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLocalization : Object {

};

// Entity Class
struct EntityLocalization : Entity {

};

// Object Struct
extern ObjectLocalization *Localization;

// Standard Entity Events
void Localization_Update();
void Localization_LateUpdate();
void Localization_StaticUpdate();
void Localization_Draw();
void Localization_Create(void* data);
void Localization_StageLoad();
void Localization_EditorDraw();
void Localization_EditorLoad();
void Localization_Serialize();

// Extra Entity Functions


#endif //!OBJ_LOCALIZATION_H
