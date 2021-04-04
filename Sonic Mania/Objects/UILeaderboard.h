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
void UILeaderboard_Update(void);
void UILeaderboard_LateUpdate(void);
void UILeaderboard_StaticUpdate(void);
void UILeaderboard_Draw(void);
void UILeaderboard_Create(void* data);
void UILeaderboard_StageLoad(void);
void UILeaderboard_EditorDraw(void);
void UILeaderboard_EditorLoad(void);
void UILeaderboard_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UILEADERBOARD_H
