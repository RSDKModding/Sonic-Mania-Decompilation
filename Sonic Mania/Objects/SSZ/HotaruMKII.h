#ifndef OBJ_HOTARUMKII_H
#define OBJ_HOTARUMKII_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectHotaruMKII;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHotaruMKII;

// Object Struct
extern ObjectHotaruMKII *HotaruMKII;

// Standard Entity Events
void HotaruMKII_Update(void);
void HotaruMKII_LateUpdate(void);
void HotaruMKII_StaticUpdate(void);
void HotaruMKII_Draw(void);
void HotaruMKII_Create(void* data);
void HotaruMKII_StageLoad(void);
void HotaruMKII_EditorDraw(void);
void HotaruMKII_EditorLoad(void);
void HotaruMKII_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HOTARUMKII_H
