#ifndef OBJ_TURRETSWITCH_H
#define OBJ_TURRETSWITCH_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTurretSwitch;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTurretSwitch;

// Object Struct
extern ObjectTurretSwitch *TurretSwitch;

// Standard Entity Events
void TurretSwitch_Update(void);
void TurretSwitch_LateUpdate(void);
void TurretSwitch_StaticUpdate(void);
void TurretSwitch_Draw(void);
void TurretSwitch_Create(void* data);
void TurretSwitch_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void TurretSwitch_EditorDraw(void);
void TurretSwitch_EditorLoad(void);
#endif
void TurretSwitch_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TURRETSWITCH_H
