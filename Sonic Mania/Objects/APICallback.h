#ifndef OBJ_APICALLBACK_H
#define OBJ_APICALLBACK_H

#include "../SonicMania.h"

//90% sure this is "DialogRunner" in plus/1.05
#if !RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectAPICallback;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityAPICallback;

// Object Struct
extern ObjectAPICallback *APICallback;

// Standard Entity Events
void APICallback_Update();
void APICallback_LateUpdate();
void APICallback_StaticUpdate();
void APICallback_Draw();
void APICallback_Create(void* data);
void APICallback_StageLoad();
void APICallback_EditorDraw();
void APICallback_EditorLoad();
void APICallback_Serialize();

// Extra Entity Functions

#endif

#endif //!OBJ_APICALLBACK_H
