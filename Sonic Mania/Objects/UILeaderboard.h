#ifndef OBJ_UILEADERBOARD_H
#define OBJ_UILEADERBOARD_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUILeaderboard;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUILeaderboard;

// Object Struct
extern ObjectUILeaderboard *UILeaderboard;

// Standard Entity Events
void UILeaderboard_Update();
void UILeaderboard_LateUpdate();
void UILeaderboard_StaticUpdate();
void UILeaderboard_Draw();
void UILeaderboard_Create(void* data);
void UILeaderboard_StageLoad();
void UILeaderboard_EditorDraw();
void UILeaderboard_EditorLoad();
void UILeaderboard_Serialize();

// Extra Entity Functions


#endif //!OBJ_UILEADERBOARD_H
