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
void DNARiser_Update(void);
void DNARiser_LateUpdate(void);
void DNARiser_StaticUpdate(void);
void DNARiser_Draw(void);
void DNARiser_Create(void* data);
void DNARiser_StageLoad(void);
void DNARiser_EditorDraw(void);
void DNARiser_EditorLoad(void);
void DNARiser_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_DNARISER_H
