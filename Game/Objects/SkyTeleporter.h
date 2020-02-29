#ifndef OBJ_SKYTELEPORTER_H
#define OBJ_SKYTELEPORTER_H

//Object Class
class SkyTeleporter : Object {

};

//Entity Class
class EntitySkyTeleporter : Entity {

};

//Entity Functions
void SkyTeleporter_Update();
void SkyTeleporter_EarlyUpdate();
void SkyTeleporter_LateUpdate();
void SkyTeleporter_Draw();
void SkyTeleporter_Setup(void* subtype);
void SkyTeleporter_StageLoad();
void SkyTeleporter_GetAttributes();

#endif //!OBJ_SKYTELEPORTER_H
