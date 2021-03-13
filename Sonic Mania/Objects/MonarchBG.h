#ifndef OBJ_MONARCHBG_H
#define OBJ_MONARCHBG_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMonarchBG;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMonarchBG;

// Object Struct
extern ObjectMonarchBG *MonarchBG;

// Standard Entity Events
void MonarchBG_Update();
void MonarchBG_LateUpdate();
void MonarchBG_StaticUpdate();
void MonarchBG_Draw();
void MonarchBG_Create(void* data);
void MonarchBG_StageLoad();
void MonarchBG_EditorDraw();
void MonarchBG_EditorLoad();
void MonarchBG_Serialize();

// Extra Entity Functions


#endif //!OBJ_MONARCHBG_H
