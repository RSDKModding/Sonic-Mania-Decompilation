#ifndef OBJ_UIVSSCOREBOARD_H
#define OBJ_UIVSSCOREBOARD_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIVsScoreboard;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIVsScoreboard;

// Object Struct
extern ObjectUIVsScoreboard *UIVsScoreboard;

// Standard Entity Events
void UIVsScoreboard_Update(void);
void UIVsScoreboard_LateUpdate(void);
void UIVsScoreboard_StaticUpdate(void);
void UIVsScoreboard_Draw(void);
void UIVsScoreboard_Create(void* data);
void UIVsScoreboard_StageLoad(void);
void UIVsScoreboard_EditorDraw(void);
void UIVsScoreboard_EditorLoad(void);
void UIVsScoreboard_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UIVSSCOREBOARD_H
