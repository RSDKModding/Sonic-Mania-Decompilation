#ifndef OBJ_AIZKINGCLAW_H
#define OBJ_AIZKINGCLAW_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectAIZKingClaw;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityAIZKingClaw;

// Object Struct
extern ObjectAIZKingClaw *AIZKingClaw;

// Standard Entity Events
void AIZKingClaw_Update(void);
void AIZKingClaw_LateUpdate(void);
void AIZKingClaw_StaticUpdate(void);
void AIZKingClaw_Draw(void);
void AIZKingClaw_Create(void* data);
void AIZKingClaw_StageLoad(void);
void AIZKingClaw_EditorDraw(void);
void AIZKingClaw_EditorLoad(void);
void AIZKingClaw_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_AIZKINGCLAW_H
