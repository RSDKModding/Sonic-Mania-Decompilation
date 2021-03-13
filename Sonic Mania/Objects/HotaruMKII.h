#ifndef OBJ_HOTARUMKII_H
#define OBJ_HOTARUMKII_H

#include "../SonicMania.h"

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
void HotaruMKII_Update();
void HotaruMKII_LateUpdate();
void HotaruMKII_StaticUpdate();
void HotaruMKII_Draw();
void HotaruMKII_Create(void* data);
void HotaruMKII_StageLoad();
void HotaruMKII_EditorDraw();
void HotaruMKII_EditorLoad();
void HotaruMKII_Serialize();

// Extra Entity Functions


#endif //!OBJ_HOTARUMKII_H
