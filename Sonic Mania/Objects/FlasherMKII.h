#ifndef OBJ_FLASHERMKII_H
#define OBJ_FLASHERMKII_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFlasherMKII;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFlasherMKII;

// Object Struct
extern ObjectFlasherMKII *FlasherMKII;

// Standard Entity Events
void FlasherMKII_Update();
void FlasherMKII_LateUpdate();
void FlasherMKII_StaticUpdate();
void FlasherMKII_Draw();
void FlasherMKII_Create(void* data);
void FlasherMKII_StageLoad();
void FlasherMKII_EditorDraw();
void FlasherMKII_EditorLoad();
void FlasherMKII_Serialize();

// Extra Entity Functions


#endif //!OBJ_FLASHERMKII_H
