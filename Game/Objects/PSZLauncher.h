#ifndef OBJ_PSZLAUNCHER_H
#define OBJ_PSZLAUNCHER_H

//Object Class
class PSZLauncher : Object {

};

//Entity Class
class EntityPSZLauncher : Entity {

};

//Entity Functions
void PSZLauncher_Update();
void PSZLauncher_EarlyUpdate();
void PSZLauncher_LateUpdate();
void PSZLauncher_Draw();
void PSZLauncher_Setup(void* subtype);
void PSZLauncher_StageLoad();
void PSZLauncher_GetAttributes();

#endif //!OBJ_PSZLAUNCHER_H
