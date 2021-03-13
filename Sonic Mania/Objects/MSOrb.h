#ifndef OBJ_MSORB_H
#define OBJ_MSORB_H

#include "../SonicMania.h"

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
void MSOrb_Update();
void MSOrb_LateUpdate();
void MSOrb_StaticUpdate();
void MSOrb_Draw();
void MSOrb_Create(void* data);
void MSOrb_StageLoad();
void MSOrb_EditorDraw();
void MSOrb_EditorLoad();
void MSOrb_Serialize();

// Extra Entity Functions


#endif //!OBJ_MSORB_H
