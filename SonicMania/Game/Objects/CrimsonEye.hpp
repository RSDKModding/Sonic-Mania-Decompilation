#ifndef OBJ_CRIMSONEYE_H
#define OBJ_CRIMSONEYE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCrimsonEye : Object {

};

// Entity Class
struct EntityCrimsonEye : Entity {

};

// Object Entity
extern ObjectCrimsonEye CrimsonEye;

// Standard Entity Events
void CrimsonEye_Update();
void CrimsonEye_LateUpdate();
void CrimsonEye_StaticUpdate();
void CrimsonEye_Draw();
void CrimsonEye_Create(void* data);
void CrimsonEye_StageLoad();
void CrimsonEye_EditorDraw();
void CrimsonEye_EditorLoad();
void CrimsonEye_Serialize();

// Extra Entity Functions


#endif //!OBJ_CRIMSONEYE_H
