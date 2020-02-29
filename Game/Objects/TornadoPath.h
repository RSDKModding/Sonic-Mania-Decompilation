#ifndef OBJ_TORNADOPATH_H
#define OBJ_TORNADOPATH_H

//Object Class
class TornadoPath : Object {

};

//Entity Class
class EntityTornadoPath : Entity {

};

//Entity Functions
void TornadoPath_Update();
void TornadoPath_EarlyUpdate();
void TornadoPath_LateUpdate();
void TornadoPath_Draw();
void TornadoPath_Setup(void* subtype);
void TornadoPath_StageLoad();
void TornadoPath_GetAttributes();

#endif //!OBJ_TORNADOPATH_H
