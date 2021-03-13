#ifndef OBJ_DNARISER_H
#define OBJ_DNARISER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDNARiser;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDNARiser;

// Object Struct
extern ObjectDNARiser *DNARiser;

// Standard Entity Events
void DNARiser_Update();
void DNARiser_LateUpdate();
void DNARiser_StaticUpdate();
void DNARiser_Draw();
void DNARiser_Create(void* data);
void DNARiser_StageLoad();
void DNARiser_EditorDraw();
void DNARiser_EditorLoad();
void DNARiser_Serialize();

// Extra Entity Functions


#endif //!OBJ_DNARISER_H
