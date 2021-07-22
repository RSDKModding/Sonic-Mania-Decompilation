#ifndef OBJ_YOYOPULLEY_H
#define OBJ_YOYOPULLEY_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectYoyoPulley;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityYoyoPulley;

// Object Struct
extern ObjectYoyoPulley *YoyoPulley;

// Standard Entity Events
void YoyoPulley_Update(void);
void YoyoPulley_LateUpdate(void);
void YoyoPulley_StaticUpdate(void);
void YoyoPulley_Draw(void);
void YoyoPulley_Create(void* data);
void YoyoPulley_StageLoad(void);
void YoyoPulley_EditorDraw(void);
void YoyoPulley_EditorLoad(void);
void YoyoPulley_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_YOYOPULLEY_H
