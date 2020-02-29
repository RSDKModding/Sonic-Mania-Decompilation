#ifndef OBJ_MAGNETSPHERE_H
#define OBJ_MAGNETSPHERE_H

//Object Class
class MagnetSphere : Object {

};

//Entity Class
class EntityMagnetSphere : Entity {

};

//Entity Functions
void MagnetSphere_Update();
void MagnetSphere_EarlyUpdate();
void MagnetSphere_LateUpdate();
void MagnetSphere_Draw();
void MagnetSphere_Setup(void* subtype);
void MagnetSphere_StageLoad();
void MagnetSphere_GetAttributes();

#endif //!OBJ_MAGNETSPHERE_H
