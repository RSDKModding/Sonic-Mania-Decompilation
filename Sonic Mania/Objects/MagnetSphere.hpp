#ifndef OBJ_MAGNETSPHERE_H
#define OBJ_MAGNETSPHERE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMagnetSphere : Object {

};

// Entity Class
struct EntityMagnetSphere : Entity {

};

// Object Entity
extern ObjectMagnetSphere MagnetSphere;

// Standard Entity Events
void MagnetSphere_Update();
void MagnetSphere_LateUpdate();
void MagnetSphere_StaticUpdate();
void MagnetSphere_Draw();
void MagnetSphere_Create(void* data);
void MagnetSphere_StageLoad();
void MagnetSphere_EditorDraw();
void MagnetSphere_EditorLoad();
void MagnetSphere_Serialize();

// Extra Entity Functions


#endif //!OBJ_MAGNETSPHERE_H
