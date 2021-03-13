#ifndef OBJ_UIDIORAMA_H
#define OBJ_UIDIORAMA_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIDiorama;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIDiorama;

// Object Struct
extern ObjectUIDiorama *UIDiorama;

// Standard Entity Events
void UIDiorama_Update();
void UIDiorama_LateUpdate();
void UIDiorama_StaticUpdate();
void UIDiorama_Draw();
void UIDiorama_Create(void* data);
void UIDiorama_StageLoad();
void UIDiorama_EditorDraw();
void UIDiorama_EditorLoad();
void UIDiorama_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_UIDIORAMA_H
