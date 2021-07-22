#ifndef OBJ_KINGCLAW_H
#define OBJ_KINGCLAW_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectKingClaw;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityKingClaw;

// Object Struct
extern ObjectKingClaw *KingClaw;

// Standard Entity Events
void KingClaw_Update(void);
void KingClaw_LateUpdate(void);
void KingClaw_StaticUpdate(void);
void KingClaw_Draw(void);
void KingClaw_Create(void* data);
void KingClaw_StageLoad(void);
void KingClaw_EditorDraw(void);
void KingClaw_EditorLoad(void);
void KingClaw_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_KINGCLAW_H
