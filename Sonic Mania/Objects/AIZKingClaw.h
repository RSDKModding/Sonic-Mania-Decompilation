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
void AIZKingClaw_Update();
void AIZKingClaw_LateUpdate();
void AIZKingClaw_StaticUpdate();
void AIZKingClaw_Draw();
void AIZKingClaw_Create(void* data);
void AIZKingClaw_StageLoad();
void AIZKingClaw_EditorDraw();
void AIZKingClaw_EditorLoad();
void AIZKingClaw_Serialize();

// Extra Entity Functions


#endif //!OBJ_AIZKINGCLAW_H
