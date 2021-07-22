#ifndef OBJ_ROLLERMKII_H
#define OBJ_ROLLERMKII_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectRollerMKII;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityRollerMKII;

// Object Struct
extern ObjectRollerMKII *RollerMKII;

// Standard Entity Events
void RollerMKII_Update(void);
void RollerMKII_LateUpdate(void);
void RollerMKII_StaticUpdate(void);
void RollerMKII_Draw(void);
void RollerMKII_Create(void* data);
void RollerMKII_StageLoad(void);
void RollerMKII_EditorDraw(void);
void RollerMKII_EditorLoad(void);
void RollerMKII_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ROLLERMKII_H
