#ifndef OBJ_FIREWORM_H
#define OBJ_FIREWORM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFireworm;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFireworm;

// Object Struct
extern ObjectFireworm *Fireworm;

// Standard Entity Events
void Fireworm_Update(void);
void Fireworm_LateUpdate(void);
void Fireworm_StaticUpdate(void);
void Fireworm_Draw(void);
void Fireworm_Create(void* data);
void Fireworm_StageLoad(void);
void Fireworm_EditorDraw(void);
void Fireworm_EditorLoad(void);
void Fireworm_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FIREWORM_H
