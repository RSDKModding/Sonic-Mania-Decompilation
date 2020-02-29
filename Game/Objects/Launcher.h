#ifndef OBJ_LAUNCHER_H
#define OBJ_LAUNCHER_H

//Object Class
class Launcher : Object {

};

//Entity Class
class EntityLauncher : Entity {

};

//Entity Functions
void Launcher_Update();
void Launcher_EarlyUpdate();
void Launcher_LateUpdate();
void Launcher_Draw();
void Launcher_Setup(void* subtype);
void Launcher_StageLoad();
void Launcher_GetAttributes();

#endif //!OBJ_LAUNCHER_H
