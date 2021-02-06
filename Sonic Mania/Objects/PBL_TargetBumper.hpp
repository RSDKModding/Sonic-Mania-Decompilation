#ifndef OBJ_PBL_TARGETBUMPER_H
#define OBJ_PBL_TARGETBUMPER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPBL_TargetBumper : Object {

};

// Entity Class
struct EntityPBL_TargetBumper : Entity {

};

// Object Entity
extern ObjectPBL_TargetBumper PBL_TargetBumper;

// Standard Entity Events
void PBL_TargetBumper_Update();
void PBL_TargetBumper_LateUpdate();
void PBL_TargetBumper_StaticUpdate();
void PBL_TargetBumper_Draw();
void PBL_TargetBumper_Create(void* data);
void PBL_TargetBumper_StageLoad();
void PBL_TargetBumper_EditorDraw();
void PBL_TargetBumper_EditorLoad();
void PBL_TargetBumper_Serialize();

// Extra Entity Functions


#endif //!OBJ_PBL_TARGETBUMPER_H
