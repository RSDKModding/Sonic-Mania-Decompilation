#ifndef OBJ_LOGOSETUP_H
#define OBJ_LOGOSETUP_H

//Object Class
class LogoSetup : Object {

};

//Entity Class
class EntityLogoSetup : Entity {

};

//Entity Functions
void LogoSetup_Update();
void LogoSetup_EarlyUpdate();
void LogoSetup_LateUpdate();
void LogoSetup_Draw();
void LogoSetup_Setup(void* subtype);
void LogoSetup_StageLoad();
void LogoSetup_GetAttributes();

#endif //!OBJ_LOGOSETUP_H
