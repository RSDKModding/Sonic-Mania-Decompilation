#ifndef OBJ_TITLESONIC_H
#define OBJ_TITLESONIC_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTitleSonic;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTitleSonic;

// Object Struct
extern ObjectTitleSonic *TitleSonic;

// Standard Entity Events
void TitleSonic_Update();
void TitleSonic_LateUpdate();
void TitleSonic_StaticUpdate();
void TitleSonic_Draw();
void TitleSonic_Create(void* data);
void TitleSonic_StageLoad();
void TitleSonic_EditorDraw();
void TitleSonic_EditorLoad();
void TitleSonic_Serialize();

// Extra Entity Functions


#endif //!OBJ_TITLESONIC_H
