#ifndef OBJ_MSORB_H
#define OBJ_MSORB_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMSOrb;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMSOrb;

// Object Struct
extern ObjectMSOrb *MSOrb;

// Standard Entity Events
void MSOrb_Update(void);
void MSOrb_LateUpdate(void);
void MSOrb_StaticUpdate(void);
void MSOrb_Draw(void);
void MSOrb_Create(void* data);
void MSOrb_StageLoad(void);
void MSOrb_EditorDraw(void);
void MSOrb_EditorLoad(void);
void MSOrb_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MSORB_H
