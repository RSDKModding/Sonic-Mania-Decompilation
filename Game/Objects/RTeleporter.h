#ifndef OBJ_RTELEPORTER_H
#define OBJ_RTELEPORTER_H

//Object Class
class RTeleporter : Object {

};

//Entity Class
class EntityRTeleporter : Entity {

};

//Entity Functions
void RTeleporter_Update();
void RTeleporter_EarlyUpdate();
void RTeleporter_LateUpdate();
void RTeleporter_Draw();
void RTeleporter_Setup(void* subtype);
void RTeleporter_StageLoad();
void RTeleporter_GetAttributes();

#endif //!OBJ_RTELEPORTER_H
