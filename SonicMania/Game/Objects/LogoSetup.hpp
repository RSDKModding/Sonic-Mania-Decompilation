#ifndef OBJ_LOGOSETUP_H
#define OBJ_LOGOSETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLogoSetup : Object {

};

// Entity Class
struct EntityLogoSetup : Entity {

};

// Object Entity
extern ObjectLogoSetup LogoSetup;

// Standard Entity Events
void LogoSetup_Update();
void LogoSetup_LateUpdate();
void LogoSetup_StaticUpdate();
void LogoSetup_Draw();
void LogoSetup_Create(void* data);
void LogoSetup_StageLoad();
void LogoSetup_EditorDraw();
void LogoSetup_EditorLoad();
void LogoSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_LOGOSETUP_H
