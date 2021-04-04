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
void FlasherMKII_Update(void);
void FlasherMKII_LateUpdate(void);
void FlasherMKII_StaticUpdate(void);
void FlasherMKII_Draw(void);
void FlasherMKII_Create(void* data);
void FlasherMKII_StageLoad(void);
void FlasherMKII_EditorDraw(void);
void FlasherMKII_EditorLoad(void);
void FlasherMKII_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FLASHERMKII_H
