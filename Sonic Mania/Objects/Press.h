#ifndef OBJ_PRESS_H
#define OBJ_PRESS_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPress;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPress;

// Object Struct
extern ObjectPress *Press;

// Standard Entity Events
void Press_Update(void);
void Press_LateUpdate(void);
void Press_StaticUpdate(void);
void Press_Draw(void);
void Press_Create(void* data);
void Press_StageLoad(void);
void Press_EditorDraw(void);
void Press_EditorLoad(void);
void Press_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PRESS_H
