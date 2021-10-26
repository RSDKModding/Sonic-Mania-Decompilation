#ifndef OBJ_VULTRON_H
#define OBJ_VULTRON_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectVultron;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityVultron;

// Object Struct
extern ObjectVultron *Vultron;

// Standard Entity Events
void Vultron_Update(void);
void Vultron_LateUpdate(void);
void Vultron_StaticUpdate(void);
void Vultron_Draw(void);
void Vultron_Create(void* data);
void Vultron_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Vultron_EditorDraw(void);
void Vultron_EditorLoad(void);
#endif
void Vultron_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_VULTRON_H
