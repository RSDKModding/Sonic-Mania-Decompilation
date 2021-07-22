#ifndef OBJ_CABLEWARP_H
#define OBJ_CABLEWARP_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCableWarp;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCableWarp;

// Object Struct
extern ObjectCableWarp *CableWarp;

// Standard Entity Events
void CableWarp_Update(void);
void CableWarp_LateUpdate(void);
void CableWarp_StaticUpdate(void);
void CableWarp_Draw(void);
void CableWarp_Create(void* data);
void CableWarp_StageLoad(void);
void CableWarp_EditorDraw(void);
void CableWarp_EditorLoad(void);
void CableWarp_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CABLEWARP_H
