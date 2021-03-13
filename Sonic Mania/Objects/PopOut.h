#ifndef OBJ_POPOUT_H
#define OBJ_POPOUT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPopOut;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPopOut;

// Object Struct
extern ObjectPopOut *PopOut;

// Standard Entity Events
void PopOut_Update();
void PopOut_LateUpdate();
void PopOut_StaticUpdate();
void PopOut_Draw();
void PopOut_Create(void* data);
void PopOut_StageLoad();
void PopOut_EditorDraw();
void PopOut_EditorLoad();
void PopOut_Serialize();

// Extra Entity Functions


#endif //!OBJ_POPOUT_H
