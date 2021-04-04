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
void PopOut_Update(void);
void PopOut_LateUpdate(void);
void PopOut_StaticUpdate(void);
void PopOut_Draw(void);
void PopOut_Create(void* data);
void PopOut_StageLoad(void);
void PopOut_EditorDraw(void);
void PopOut_EditorLoad(void);
void PopOut_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_POPOUT_H
